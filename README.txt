HOW TO RUN THE PROGRAM
1.) Navigate to the project folder location: “smhgzd_jtvd4_cgcnbf_project”
2.)Type the command “make”
3.)Run the executable named “final” followed by the desired input file and hit return

	Example: mwc-040216:smhgzd_jtvd4_cgcnbf_project Shelby$ make
		 gcc src/final.c -g -c
		 gcc src/vector.c -g -c
		 gcc src/graph.c -g -c
		 gcc final.o vector.o graph.o -g -o final
		 mwc-040216:smhgzd_jtvd4_cgcnbf_project Shelby$ ./final room.txt
			
		 … results will be displayed here …

INTERPRETING THE RESULTS

-What will be displayed are two sets of mazes that depict the input files initial room design and setup

-Looking at the mazes, note which maze is for which robot by looking at the indicator above each maze. It will describe which specific robot’s solution you are viewing below it.

-Additionally, you will see just below each graph a conclusion of the path result you see such as “Exit found!” of “Exit not found” 

-First it will exclaim whether or not each robot has found an exit, secondly, it will describe how many spaces/moves it took for the robot to make it to the intended goal.

-Any problems including parsing, etc. will exit the program immediately and print the error message to the screen.
						