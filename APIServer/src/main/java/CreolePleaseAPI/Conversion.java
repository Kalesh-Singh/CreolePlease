package CreolePleaseAPI;

import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.lang.StringBuilder;
import java.lang.ProcessBuilder;
import java.lang.Process;
import java.util.concurrent.TimeUnit;


/*Stores original and converted text*/
public class Conversion {

    private final long id;
    private final String text;
    private final StringBuilder converted_text;

    public Conversion(long id, String text) {
        this.id = id;
        this.text = text;
        this.converted_text = new StringBuilder("");

        /*Convert text*/

        /*Write text to file*/
        try {
            BufferedWriter writer = new BufferedWriter(new FileWriter("../CreolePleaseConverter/infile.txt"));
            writer.write(text);
            writer.close();
        }catch(IOException e){
            e.printStackTrace();
        }
            
        /*Run CreolePlease Conversion Code*/
        try{
            String[] args = new String[] {"/bin/bash", "-c", "cd ..; cd CreolePleaseConverter; ./run.sh;"};
            Process proc = new ProcessBuilder(args).start();
        }
        catch(IOException e) {
            e.printStackTrace();
        }

        /*Wait for C++ conversion code to finish. Poorly done. To be replaced.*/
        try{
        TimeUnit.SECONDS.sleep(5);
        }catch(InterruptedException e) {
            e.printStackTrace();
        }
        
        /*Read text from file*/
        String fileName = "../CreolePleaseConverter/outfile.txt";
        String line = null;

        try {
            FileReader fileReader = new FileReader(fileName);
            // Always wrap FileReader in BufferedReader.
            BufferedReader bufferedReader = 
                new BufferedReader(fileReader);

            while((line = bufferedReader.readLine()) != null) {
                if(line.equals("")){
                    converted_text.append(" ");
                }
                else{
                    converted_text.append(line);
                }
                  
            }   
            bufferedReader.close();         
        }catch(IOException e) {
            e.printStackTrace();
        }

    }

    public long getId() {
        return id;
    }

    public String getText() {
        return text;
    }

    public String getConvertedText() {
        return converted_text.toString();
    }
}