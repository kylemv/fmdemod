# fmdemod
FM Modulator and Demodulator for use in redhawk using liquid-dsp library.  

To use these components

1) Install redhawk:

  -http://redhawksdr.github.io/Documentation/mainch2.html#x4-60002

2) Install liquid-dsp

-Clone the repository at lmcadoo2/liquid-dsp

	  	git clone git://github.com/lmcadoo2/liquid-dsp.git

-Enter the directory

-Run the following
	
			./bootstrap.sh
		 	./configure
		 	make
	  	sudo make install

3) Get the component code

-Clone this repository

		 git clone git://github.com/kylemv/fmdemod.git

-Import the project into eclipse

	  	-File->Import->C/C++->Existing Code as Makefile Project
	
	  	-Browse to the folder in the 'Existing Code Location' box
	
	  	-Finish

-Build the project

  		-Right click the project folder and select 'Build Project'

-Export to the 'Target SDR'

  		-Drag the project folder to 'Target SDR' in the SCA Explorer view

4) The component is now ready for use in Redhawk. For more information on components in Redhawk, see chapters 3 and 4 in the documentation located here:

  -http://redhawksdr.github.io/Documentation/main.html

For more information on Redhawk see http://redhawksdr.github.io/Documentation/index.html

For more information on liquid-dsp see liquidsdr.org
