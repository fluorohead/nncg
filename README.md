# NNCG "Network Node Configuration Generator"

![nncg screenshot](screenshot.png)

<b>A simple templates-based config generator for network devices.</b>

Usefull for industrial type equipment, like : Routers, Switches, DSLAM, OLT, WDM and etc...

Designed to facilitate routine operations for network engineers during mass configuration of similar devices.
Once you create a template, you can reuse it further, changing variables such as hostname, IP addresses, masks, mac addresses, network protocol settings,
names and descriptions of interfaces. The type of the variable doesn't matter.

The application works on the principle of substituting data values described in the template header using key values enclosed
in curly braces (tunable). The template file is a simple text file, but the file header must conform to a specific format for the application to function correctly.
This file is prepared by an engineer in any text editor. The example <b>"template_example.txt"</b> is used as a basis.

The application also provides a function for saving entered values in CSV format in case you do not have to re-enter rarely changed variables each time.

Having a template file and once saved a CSV dump, you can make your life much easier, and in the time freed from routine, drink coffee or read documentation about network technologies.

The software is written using the Qt 6.6.1 framework and uses icons from https://icons8.com/


<b>List of variable types and description of checks performed when entering values.</b>

Domname - checking the correctness of the domain name, max. length 253 characters, only Latin alphabet and hyphens
Prompt - prompt in the device console, max. 64 Unicode characters
Text - plain text without language restrictions, max. length 255 unicode characters
IPv4 - check for compliance with IPv4 address
IPv6 - checking for compliance with IPv6 address
Unsigned - unsigned 32-bit integer in the range from 0 to 4_294_967_295
Password - password, hidden input field, max. length 128 unicode characters
MASKv4 - checking the correctness of the mask for IPv4 in dotted format
MASKv4Len - IPv4 mask length, integer from 0 to 32
MASKv6Len - mask length for IPv6 address, integer from 0 to 128
WildcardV4 - inverted wildcard bitmask for IPv4, check for correctness
WildcardV6 - inverted wildcard bitmask for IPv6, check for correctness
Hash - hash sequence, only Latin and special characters, max. length 128 characters

Capital and small letters in type names do not play a role. For example, the following names are identical: Domname, dOMName, DonName, etc.