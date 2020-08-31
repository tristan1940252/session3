package com.tristanbrault;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;

public class Main {

    public static void main(String[] args) throws IOException {
        RandomAccessFile file = null;
        String fileName = "default";
        long offset = 0;
        long length = 0;
        long stringLength = 4;
        boolean fileExist = true;
        boolean text = false;

        for (int i = 0; i < args.length; i++) {
            if (args[i].equals("-f")) {
                fileName = args[++i];
                System.out.println(fileName + " filename");
            }
            try{
                file = new RandomAccessFile(fileName,"r");
            }
            catch (FileNotFoundException e){
                printUsage();
                fileExist = false;
            }
            //if (fileExist){
                length = file.length();
                if(args[i].equals("-o")){
                    if ((Long.parseLong(args[i+1]) >= 0) && (Long.parseLong(args[i+1]) < file.length())){
                        offset = Long.parseLong(args[i+1]);
                    }
                    System.out.println(offset + " offset");
                }
                else if (args[i].equals("-l")){
                    if (Long.parseLong(args[i+1]) > 0 && Long.parseLong(args[i+1]) < (file.length() - offset)){
                        length = Long.parseLong(args[i+1]);
                    }
                    System.out.println(length + " length");
                }
                else if(args[i].equals("-s")){
                    text = true;
                    try{
                        stringLength = Long.parseLong(args[i+1]);
                    }
                    catch (Exception e){
                        stringLength = 4;
                    }
                }
                else{
                    text = false;
                }
            //}
        }
        if (text){
            printText(file,offset,length,stringLength);
        }
        else
            printData(file, offset, length);
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
                        if(chiffre != 10 && chiffre != 13){
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
        System.out.println("-o est la position de départ (facultatif)");
        System.out.println("-l est le nombre de caractère à observer (facultatif)");
        System.out.println("-f est le nom du fichier à lire (obligatoire)");
    }

    public static void printText(RandomAccessFile file, long offset, long length, long max) throws IOException{
        StringBuilder lolipop = new StringBuilder();
        for (long i = offset; i < length; i++){
            file.seek(i);
            int chiffre = file.read();
            //if((chiffre >= 48 && chiffre <= 57) || (chiffre >= 65 && chiffre <= 90) || (chiffre >= 97 && chiffre <= 122) || chiffre == 32){
            if (chiffre > 7 && chiffre < 128){
                //System.out.print("\u001B[37m" + (char)chiffre);
                lolipop.append((char) chiffre);
            }
            else{
                //System.out.print(".");
                if (chiffre == 0 && lolipop.length() > max){
                    System.out.print(lolipop);
                }
                else{
                    lolipop = new StringBuilder();
                }
            }
        }
        System.out.println();
    }


    public static String hexAddress(long address){
        String hexA = "";
        String hex =  String.valueOf(address / 16);
        switch (hex.length()){
            case 1: hexA = "0x0000" + hex + "0 "; break;
            case 2: hexA = "0x000" + hex + "00 "; break;
            case 3: hexA = "0x00" + hex + "000 "; break;
            case 4: hexA = "0x0" + hex + "0000 "; break;
            case 5: hexA = "0x" + hex + "00000 "; break;
        }
        hexA = "\u001B[33m" + hexA;
        return hexA;
    }
}
