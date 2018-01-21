# CreolePlease

To run:

1.Pull the repository to your system.
2.Using the terminal, navigate to the CreolePleaseConverter folder and run the following commands:

$ make
$ chmod +x run.sh

3.Navigate to the APIServer folder and run the following command:

$ gradle bootRun

4.The server should come online and can be accessed at the address

localhost:8080/convert


5.To try different strings to be converted, modify the text variable as follows:

localhost:8080/convert?text=This is a test.
