SPI NOR Flash
==================

SPI NOR Flash is middleware which provides API to flash access.


Architecture and provided functionality
---------------------------------------

All SPI NOR Flashes are composed of two parts:

* Common interface

Common interface part contains interface that is shared by all specific flash. 
Common interface is a driver for abstract flash operation. This allows applications
to use common interface of SPI NOR flash despite which one is used in a system
and simplifies switching to another flash.

* Flash specific

Implementation for specific flash.
* Flash erase
* Flash read/write
* Flash enable/disable xip mode

Dependencies
------------

* An instance of the QSPI driver is used by this middleware.

