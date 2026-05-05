I have decided to write this little project because windows wouldn't let me create files hastily with the extensions that I want. So, I resorted to the only way I thought it could be fixed. Which is by writing my own File Creation Utility (fcu) tool in C.

I have used a bunch of different things in this program, such as a poor man's hash map or a tiny tokenizer which makes tokens and  then assigns these tokens with their respective meanings using the map we have. I tried to build the architecture to specifically be very fast and lightweight as much as I can. Since speed is of the essence, I don't do any Run-time allocations, there could be some memory overhead that Might happen. Given, it is in insignificant amounts. 

# <ins> COMMAND LIST:</ins>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~UPDATE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

To update the program from an old version to a new version, Just run the install.ps1 script once again after installing the new version from it's folder.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# <ins> COMMAND LIST:</ins>
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~The commands~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

fcu -> This is the way you can call the program after running the install.ps1 script. If you won't provide any more arguments for the program, it will just create a file called 'tmp.txt' in the directory where you called the program. For now, the program only append / creates the file name you call, But if the file exists before, it will ask you if you would like to append to the file that already exists.


-help -> This command is pretty much self explanatory. When you call this command, it will give you the latest verison's command list and quick eplanations on what they do.

-n -> This command is the short for "Name". If you call this command by itself and then enter a name after, such as: ' >fcu -n "Lorem" '. The output file will be: 'Lorem.txt'.

-e -> This command is the short for "Extension". If you call this command by itself and then enter an extension after, such as: ' >fcu -e ".ipsum" '. The output file will be: 'tmp.ipsum'. Which Windows sees as a i.IPSUM file and will ask you to find a proper app to open it up with if you double click on it.

-i -> This command is the short for "Initiate". If you call this command by itself and then enter any text after, such as: ' >fcu -i "Hello, World !" '. The output file will be: 'tmp.txt', and if you double click the file, you'll see that "Hello, World !" is indeed inside of the file. (1024 Characters are the max buffer area that's allowed for writing in the file with this command.)

-p -> This command is the short for "Path". If you call this command by itself and then enter any text after, such as: ' >fcu -p "c:\Users\helloMe\Documents\" '. The output file will be created at the location you just provided instead of the current working directory. (PRO TIP: if you don't want to manually enter all the path and aware of the general path. You can just enter the beginning such as ":c\" and enter the first 2-3 letters of the directories you are going to navigate then press tab for it to auto-fill the rest, such as: "c:\use" - Press tab -> "c:\Users\". you can fill out the whole path a lot faster this way instead of manually writing the whole path.)

-of -> This command is the short for "Open File". If you call this command by itself, what it will do is, create a 'tmp.txt' file where your CWD is and then open the file right after for modification. (Can't do no shell injections either since I do a checking if it's an acutal path or something else before passing the arguments into the program :) )

-od -> This command is the short for "Open Directory". If you call this command by itself, what it will do is, it will create a 'tmp.txt' file where your CWD is and then open up the Directory in which you created the file (Key part is where you created the file, so -p does affect this command if used together.).

-dof -> This command is the short for "Don't Open File". If you call this command, It will basically just not create a file or open it if it exists in the provided direcetory. The main usage for this command is when you want to navigate directories and open up the GUI screens of the said directories without creating tmp files to delete later. (More usages may come in the future.)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

# (Current version of the program is v1.4.5. So if you're reading this in a more recent release, Some of these might be changed or altered.)
