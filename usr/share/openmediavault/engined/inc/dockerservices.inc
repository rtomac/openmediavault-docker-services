<?php

abstract class OMVModuleDockerService extends \OMV\Engine\Module\ServiceAbstract
    implements \OMV\Engine\Module\IServiceStatus
{
    private $containerName = NULL;
    private $serviceName = NULL;

    function __construct($containerName, $serviceName) {
        $this->containerName = $containerName;
        $this->serviceName = $serviceName;
    }

    public function getName()
    {
        return "dockersvc" . $this->containerName;
    }

    public function applyConfig()
    {
    }

    public function startService()
    {
        $this->startContainer();
    }

    public function stopService()
    {
        $this->stopContainer();
    }

    public function getStatus()
    {
        $status = $this->getContainerStatus();
        return array(
            "name" => $this->getName(),
            "title" => gettext($this->serviceName),
            "enabled" => $status["enabled"],
            "running" => $status["running"]
        );
    }

    private function getContainerStatus()
    {
        $cmd = new \OMV\System\Process(
            "docker",
            "inspect",
            "-f",
            "{{.State.Running}}:{{.State.Paused}}:{{.State.Restarting}}:{{.HostConfig.RestartPolicy.Name}}",
            $this->containerName);
        $cmd->setRedirect2to1();
        $cmd->setQuiet();
        $cmd->execute($output, $exitCode);

        $enabled = FALSE;
        if ($exitCode == 0)
        {
            $vals = split(":", $output[0]);
            if (count($vals) == 4)
            {
                $running = ($vals[0] == "true");
                $paused = ($vals[1] == "true");
                $restarting = ($vals[2] == "true");
                $restartsAlways = ($vals[3] == "always");

                $started = $running || $paused || $restarting;
                $enabled = $started || $restartsAlways;
            }
        }

        return [
            "enabled" => (bool)$enabled,
            "started" => (bool)$started,
            "running" => (bool)$running
        ];
    }

    public function startContainer()
    {
        $status = $this->getContainerStatus($this->containerName);

        if ($status["enabled"] && !$status["started"])
            return;

        $cmd = new \OMV\System\Process("docker", "start", $this->containerName);
        $cmd->setRedirect2to1();
        $cmd->execute();
    }

    public function stopContainer()
    {
        $status = $this->getContainerStatus($this->containerName);

        if ($status["enabled"] && $status["started"])
            return;

        $cmd = new \OMV\System\Process("docker", "stop", $this->containerName);
        $cmd->setRedirect2to1();
        $cmd->execute();
    }
}
