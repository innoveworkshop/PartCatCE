# PartCat for Windows CE

A port of [PartCat](https://github.com/innoveworkshop/PartCat) to the Windows
CE platform.

![Main Screenshot](/Screenshots/2020-06-30.png)


## Development Requirements

This project depends on the amazing [STL for eMbedded Visual C++](http://users.libero.it/g.govi/stlport_ce_en.html),
which is included in the project in case the webpage goes down in the future.
Because of this, in order to compile this project you must add the STL directory
to the eVC++ include path as described in the STL readme file:

> From eVC++, add the directory name to the include search path (Tools/Options.../Directories). Note that you must repeat this step for all desired "CPUs" / "Platform" combinations.


## License

This project is licensed under the **MIT License**.
