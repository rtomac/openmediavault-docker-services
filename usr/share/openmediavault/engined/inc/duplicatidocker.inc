<?php

class DuplicatiDockerHelper
{
    const CONFIG_ID = "conf.service.duplicatidocker";

    public function getConfiguration()
    {
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get(self::CONFIG_ID);
        return $object->getAssoc();
    }

    public function setConfiguration($config)
    {
        $db = \OMV\Config\Database::getInstance();
        $object = $db->get(self::CONFIG_ID);
        $object->setAssoc($config);
        $db->set($object);

        return $object->getAssoc();
    }

    public function startContainer()
    {
        $containerName = $this->getContainerName();
        $status = $this->getContainerStatus($containerName);

        if (!$status["stopped"])
            return;

        $cmd = new \OMV\System\Process("docker", "start", $containerName);
        $cmd->setRedirect2to1();
        $cmd->execute();
    }

    public function stopContainer()
    {
        $containerName = $this->getContainerName();
        $status = $this->getContainerStatus($containerName);

        if ($status["stopped"])
            return;

        $cmd = new \OMV\System\Process("docker", "stop", $containerName);
        $cmd->setRedirect2to1();
        $cmd->execute();
    }

    public function getContainerName()
    {
        $settings = $this->getConfiguration();
        return $settings["container-name"];
    }

    public function getContainerStatus($containerName)
    {
        $cmd = new \OMV\System\Process("docker", "inspect", "-f", "{{.State.Running}}:{{.State.Paused}}:{{.State.Restarting}}:{{.HostConfig.RestartPolicy.Name}}", $containerName);
        $cmd->setRedirect2to1();
        $cmd->setQuiet();
        $cmd->execute($output);

        $exists = FALSE;
        if (count($output) > 0)
        {
            $vals = split(":", $output[0]);
            if (count($vals) == 4)
            {
                $exists = TRUE;

                $running = ($vals[0] == "true");
                $paused = ($vals[1] == "true");
                $restarting = ($vals[2] == "true");
                $restartsAlways = ($vals[3] == "always");

                $stopped = !$running && !$paused && !$restarting;
                $enabled = $running || $paused || $restarting || $restartsAlways;
            }
        }

        return [
            "exists" => $exists,
            "running" => (bool)$running,
            "paused" => (bool)$paused,
            "restarting" => (bool)$restarting,
            "stopped" => (bool)$stopped,
            "restartsAlways" => (bool)$restartsAlways,
            "enabled" => (bool)$enabled,
            "statusString" => $statusString,
        ];
    }

    public function getStatusString($containerName, $status)
    {
        $statusString = "";

        if (!$status["exists"]) $statusString = "does not exist";
        else if ($status["running"]) $statusString = "is running";
        else if ($status["paused"]) $statusString = "is paused";
        else if ($status["restarting"]) $statusString = "is restarting";
        else $statusString = "is stopped";

        return "Container '" . $containerName . "' " . $statusString . ".";
    }
}
