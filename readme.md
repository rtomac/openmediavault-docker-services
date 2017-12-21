Introduction
============

This is a simple set of OpenMediaVault modules that will report enabled/running status in the service health dashboard for services running as Docker containers on the OpenMediaVault server.

It currently supports two services/containers (but can easily be expanded to cover more):
- Duplicati
- Hamachi

They require manual installation (file copy). Tested with OMV 3.x.

How it works
============

This is an implementation of `\OMV\Engine\Module\IServiceStatus` that uses [docker inspect](https://docs.docker.com/engine/reference/commandline/inspect/) commands to interrogate the existence and status of the containers.

The containers are referenced by their service name (e.g. 'duplicati').

There is a common base class in `engined/inc/dockerservices.inc` which implements this logic.

Individual service modules live in `engined/module`, e.g. `engined/module/dockerservice-duplicati`. They can be implemented very simply as a subclass with just a constructor implementation.

Installation
============

Install files and restart services
----------------------------------

From a remote machine:
```
git clone https://github.com/rtomac/openmediavault-docker-services.git
rsync -av -essh ./openmediavault-docker-services/usr/share/openmediavault/ root@server:/usr/share/openmediavault/
ssh root@server service openmediavault-engined restart
```

To uninstall
------------

From a remote machine:
```
ssh root@server rm /usr/share/openmediavault/engined/*/dockerservice*.inc
ssh root@server service openmediavault-engined restart
```

Troubleshooting
---------------

If you run into trouble (OMV daemon fails to start), run the process in debug mode:
```
omv-engined -df
```
