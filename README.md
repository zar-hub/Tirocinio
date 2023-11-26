# Tirocinio 

## Bug fixed:
- There was a problem in `analisi.C` where the barrel and endcaps positions where not tested with `abs()` function. This resulted in more events beeing passed in the barrel than the ones in the funciton `filters::GoodWP80(int)`.


## Description
![graph](canvas.png)

Main classes:






## Notes
- I chanded fitBarrel/EndcapsPassed to a double sided exp because using the two sided gaussian with a peak it would not converge.

## Getting Started

To get started with this project, clone the repository and open the project in an IDE that supports C++ and ROOT, such as Visual Studio Code.

## Contributing

Contributions are welcome. Please open an issue to discuss your ideas before making a change.

## License

This project is licensed under the MIT License - see the LICENSE file for details.