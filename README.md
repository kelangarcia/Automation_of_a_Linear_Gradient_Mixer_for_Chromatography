# Automation-of-a-Linear-Gradient-Mixer-for-Chromatography
This repository has all the files and documentation (3D Case, Code, Circuit Analysis, Circuit Simulations, Procedure, Schematics,Results etc.) that were used to create a machine that automates the process of mixing gradients for chromatography. A so called Gradient Mixer Machine.

All the folders and files included are explained below:

- "Automation of Gradient Mixer for Chromatography.pdf": Technical motivation and explanation of the machine. Includes explanation of the final design, circuit, code, etc... works.

- "3D Case": This folder includes the 3D files exported into .stl (This is the format used for 3D printing). Just upload this file to a 3D printer or sent it to a production company.

- "Circuit": It includes the "Circuit_Diagram.pdf" file, this is supporting material for when you are soldering the components. It is a good guide. It also includes the Datasheets for the most important hardware components used. Like the solenoid valve, and transistor.

- "Code": This folder includes three important files. Two gradient mixer codes and a zip file with the needed libraries. The zip file needs to be installed into your Arduino IDE (read the instruction set for more), once it is installed. You can upload one of the two Gradient Mixer code provided. First try with the original code and test the machine. If you notice that when there is a press in the touch screen it detects the touch but in the inverted y-axis position, then upload the second code (LCD inverted code).

- "Instruction Sets": Two .pdf files, one is the instructions for the user, and the other has the instructions for making the gradient mixer.

- "PCB": Includes the .fzz file for editing the PCB design, and it also includes the export files of the pcb in three diferent formats (.pdf, .svg, Gerber Files). Depending on the company you are printing the PCB with, they will request for the type of file. The most common is the Gerber File. The file "KGGradient_Mixer_06072021.zip" has all the Gerber Files compressed into a zip folder, you can send this zip file to the production company, or create your own zip file with the provided Gerber files.
 
- "EXTRA": Extra material

- "Gradient Mixer Price": List of materials with their average price inthe market, giving how much does this gradient mixer construction cost.

Date: 01.21.23
