Introduction
============

This is an OpenMediaVault plugin for Duplicati when Duplicati is running in a Docker container. Provides a service module (that reports health) and service settings (that link out to the Duplicati UI).

There is an "official" plugin for Duplicati in the OMV extras, but it requires that Duplicati be installed on the host (which is messy).
https://github.com/OpenMediaVault-Plugin-Developers/openmediavault-duplicati

This doesn't currently support making a .deb file for proper plugin installation (see below for manual installation).

Tested with OMV 3.x.

Installation
============

Seed config file
----------------

Add the following node to /etc/openmediavault/config.xml on the server (OMV requires this):
```
<config>
  <services>
    <duplicatidocker>
      <container-name>duplicati</container-name>
      <port>8200</port>
    </duplicatidocker>
  </services>
</config>
```

Install files and restart services
----------------------------------

From a remote machine:
```
git clone https://github.com/rtomac/openmediavault-duplicati-docker.git
rsync -av -essh ./openmediavault-duplicati-docker/usr/ root@server:/usr/
rsync -av -essh ./openmediavault-duplicati-docker/var/ root@server:/var/
ssh root@server service openmediavault-engined restart
ssh root@server rm /var/cache/openmediavault/*admin_js.json
```

Troubleshooting
---------------

If you run into trouble (omv daemon fails to start), you can run the process in debug mode:
```
omv-engined -df
```
