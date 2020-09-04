package com.tristanbrault;
import javax.print.DocFlavor;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

public class Main {

    public static void main(String[] args) throws IOException {
        RandomAccessFile file = null;
        String fileName = "default";
        long offset = 0;
        long length = -1;
        long stringLength = 4;
        boolean fileExist = true;
        boolean text = false;
        boolean infos = false;

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-f")) {
                fileName = args[++i];
                System.out.println(fileName + " filename");
                try{
                    file = new RandomAccessFile(fileName,"r");
                }
                catch (FileNotFoundException e){
                    printUsage();
                    fileExist = false;
                }
            }

        }
        if (fileExist){
            for (int i = 0; i < args.length; i++) {
                switch (args[i]) {
                    case "-o" -> {
                        if ((Long.parseLong(args[i + 1]) >= 0) && (Long.parseLong(args[i + 1]) < file.length())) {
                            offset = Long.parseLong(args[i + 1]);
                        }
                        System.out.println(offset + " offset");
                    }
                    case "-l" -> {
                        if (Long.parseLong(args[i + 1]) > 0 && Long.parseLong(args[i + 1]) < (file.length() - offset)) {
                            length = Long.parseLong(args[i + 1]);
                        }
                        System.out.println(length + " length");
                    }
                    case "-s" -> {
                        text = true;
                        try {
                            stringLength = Long.parseLong(args[i + 1]);
                        } catch (Exception e) {
                            stringLength = 4;
                        }
                    }
                    case "-i" -> {
                        infos = true;
                    }

                }
            }
            if (length == -1){
                length = file.length() - offset;
            }
            if (text){
                printText(file,offset,length,stringLength);
            }
//            else  if (infos){
//                printInfos(file);
//            }
            else{
                printData(file, offset, length);
            }
            if (infos){
                printInfos(file);
            }
        }

    }

    public static void printData(RandomAccessFile file, long offset, long length) throws IOException {
       long index = (offset/16) * 16;
       System.out.println("\u001B[33m          0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f  ASCII");
         while (file.read() != -1 && index <= (offset + length)){
             StringBuilder texte = new StringBuilder();
             //Hexadecimal
             System.out.print(hexAddress(index));
             for (int i = 0; i < 16; i++){
                file.seek(index);
                int chiffre = file.read();
                if(chiffre > 0 && index < (offset + length)){
                    if(index >= offset){
                        if(chiffre > 17){
                            System.out.print("\u001B[36m"  + Integer.toHexString(chiffre) + " ");
                            texte.append((char) chiffre);
                        }
                        else{
                            System.out.print("\u001B[36m" + "0"  + Integer.toHexString(chiffre) + " ");
                            texte.append(".");
                        }
                    }
                    else{
                        System.out.print("   ");
                        texte.append(" ");
                    }

                }
                else{
                    System.out.print("   ");
                    texte.append(" ");
                }
                index++;
             }
             //Texte
             System.out.println("\u001B[37m" + texte);
        }
    }

    public static void printUsage(){
        System.out.println("-f est le nom du fichier à lire (obligatoire)");
        System.out.println("-o est la position de départ (facultatif)");
        System.out.println("-l est le nombre de caractère à observer (facultatif)");
        System.out.println("-s est tranforme l'hexadecimal en chaine de charactère, mettre un chiffre pour spécifier la taille minimum des chaines (faculatitf)");
        System.out.println("-i donne des informations sur le fichier (facultatif)");

    }

    public static void printText(RandomAccessFile file, long offset, long length, long min) throws IOException{
        String chaine = "";
        for (long i = offset; i < length; i++){
            file.seek(i);
            int chiffre = file.read();
            //if((chiffre >= 48 && chiffre <= 57) || (chiffre >= 65 && chiffre <= 90) || (chiffre >= 97 && chiffre <= 122) || chiffre == 32){
            if (chiffre > 7 && chiffre < 128){
                //System.out.print("\u001B[37m" + (char)chiffre);
                chaine += (char) chiffre;
            }
            else{
//                System.out.print("\u001B[33m" + Long.toHexString(chiffre) + " ");
                if (chiffre == 0 && chaine.length() >= min){
                    System.out.println(chaine);
                }
                chaine = "";
            }
        }
        System.out.println();
    }

    public static void printInfos(RandomAccessFile file) throws IOException{
        //int[] signature = {80,69,0,0};
        int[] signature = {69,76,70};
        //Adresse OS
        //Adresse Processeur
        //Signature Processeur
        int response = 0;
        byte count = 0;
        int i = 0;
        String cpu = "";
        String os = "";

    while(os.equals("")){
        file.seek(i);
        int value = file.read();
        if (value == signature[count]){
            if (count == signature.length - 1){
                response = i;
                os = "OS: Windows";
                count--;
            }
            count++;
        }
        else{
            count = 0;
        }

        if (!os.equals("")){
            System.out.println("\u001B[52m" + response);
            for (int j = 12; j > 10; j--){
                file.seek(response + j);
                cpu += Integer.toHexString(file.read());
            }
            cpu = switch (cpu) {
                case "8664" -> "Machine: x64";
                case "aa64" -> "Machine: ARM64";
                case "3e" -> "AMD64";
                case "b7" -> "ARM64";
                default -> cpu;
            };
        }
        i++;
    }

        System.out.println(os);
        System.out.println(cpu);
    }


    public static String hexAddress(long address){
        String hexA = "";
        String hex =  String.valueOf(address / 16);
        switch (hex.length()){
            case 1: hexA = "0x0000" + hex + "0 "; break;
            case 2: hexA = "0x000" + hex + "0 "; break;
            case 3: hexA = "0x00" + hex + "0 "; break;
            case 4: hexA = "0x0" + hex + "0 "; break;
            case 5: hexA = "0x" + hex + "0 "; break;
        }
        hexA = "\u001B[33m" + hexA;
        return hexA;
    }
}