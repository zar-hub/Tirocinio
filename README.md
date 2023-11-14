# Tirocinio 

This is a C++ project that uses the ROOT framework for data analysis in particle physics. The project includes various fit functions and analysis methods to study properties of particles.

## Description

This project includes several key components:

- Fit functions: These are defined in `fitFunctions.h` and include various fit functions used in the analysis.
- Analysis code: The main analysis code is contained in `analisi.C` and `analisi.h`.
- Data: The data for the analysis is stored in ROOT files.

## TODO

- [x] Improve fit functions.
- [ ] Improve the plotting code.
- [ ] Write tests for the analysis code.
- [ ] Document the code.
- [ ] fitEndcapsFailed does not converge
- [ ] remove noise from the failed graph
- [ ] find efficiency

## Notes
- I chanded fitBarrel/EndcapsPassed to a double sided exp because using the two sided gaussian with a peak it would not converge.

## Getting Started

To get started with this project, clone the repository and open the project in an IDE that supports C++ and ROOT, such as Visual Studio Code.

## Contributing

Contributions are welcome. Please open an issue to discuss your ideas before making a change.

## License

This project is licensed under the MIT License - see the LICENSE file for details.