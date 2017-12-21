<?php

class OMVRpcServiceDuplicatiDocker extends \OMV\Rpc\ServiceAbstract
{
    public function getName()
    {
        return "DuplicatiDocker";
    }

    public function initialize()
    {
        $this->registerMethod("getSettings");
        $this->registerMethod("setSettings");
    }

    public function getSettings($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);

        $helper = new DuplicatiDockerHelper();

        $settings = $helper->getConfiguration();
        $containerName = $settings["container-name"];

        $status = $helper->getContainerStatus($containerName);

        $settings["container-running"] = $status["running"];
        $settings["container-status"] = $helper->getStatusString($containerName, $status);

        return $settings;
    }

    public function setSettings($params, $context)
    {
        // Validate the RPC caller context.
        $this->validateMethodContext($context, ["role" => OMV_ROLE_ADMINISTRATOR]);

        // Validate the parameters of the RPC service method.
        $this->validateMethodParams($params, "rpc.duplicatidocker.setsettings");

        $helper = new DuplicatiDockerHelper();
        return $helper->setConfiguration($params);
    }
}
