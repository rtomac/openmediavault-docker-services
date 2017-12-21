<?php

class OMVModuleDuplicatiDocker extends \OMV\Engine\Module\ServiceAbstract
    implements \OMV\Engine\Notify\IListener, \OMV\Engine\Module\IServiceStatus
{
    public function getName()
    {
        return "duplicatidocker";
    }

    public function applyConfig()
    {
    }

    public function startService()
    {
        $helper = new DuplicatiDockerHelper();
        $helper->startContainer();
    }

    public function stopService()
    {
        $helper = new DuplicatiDockerHelper();
        $helper->stopContainer();
    }

    public function getStatus()
    {
        $helper = new DuplicatiDockerHelper();
        $containerName = $helper->getContainerName();
        $status = $helper->getContainerStatus($containerName);

        return array(
            "name" => $this->getName(),
            "title" => gettext("Duplicati"),
            "enabled" => $status["enabled"],
            "running" => $status["running"]
        );
    }

    public function bindListeners(\OMV\Engine\Notify\Dispatcher $dispatcher)
    {
    }
}
