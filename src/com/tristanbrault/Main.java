package com.tristanbrault;
import javax.print.DocFlavor;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.RandomAccessFile;



public class Main {

    public static final int[] SIGNATURE_WINDOWS = {0x50,0x45,0x00,0x00};
    public static final int CPU_OFFSET_WINDOWS = 1;
    public static final int CPU_LENGTH_WINDOWS = 2;
    public static final String CPU_X64_WINDOWS = "8664";
    public static final String CPU_ARM64_WINDOWS = "aa64";

    public static final int[] SIGNATURE_MAC = {0xcf,0xfa,0xed,0xfe};
    public static final int CPU_OFFSET_MAC = 1;
    public static final int CPU_LENGTH_MAC = 1;
    public static final int CPU_X64_MAC = 0x07;
    public static final int CPU_ARM64_MAC = 0x12;

    public static final int[] SIGNATURE_LINUX = {0x7f,0x45,0x4c,0x46};
    public static final int CPU_OFFSET_LINUX = 15;
    public static final int CPU_LENGTH_LINUX = 1;
    public static final String CPU_X64_LINUX = "3e";
    public static final String CPU_ARM64_LINUX = "b7";

    public static final int[][] ARRAY_SIGNATURE = {SIGNATURE_WINDOWS,SIGNATURE_MAC,SIGNATURE_LINUX};

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
            else  if (infos){
                printInfos(file);
            }
            else{
                printData(file, offset, length);
            }
//            if (infos){
//                printInfos(file);
//            }
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

    //https://docs.microsoft.com/fr-ca/windows/win32/debug/pe-format?redirectedfrom=MSDN#machine-types

    //https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#File_header

    //https://opensource.apple.com/source/xnu/xnu-792.13.8/osfmk/mach/machine.h

    //https://stackoverflow.com/questions/27669766/how-to-read-mach-o-header-from-object-file
    public static void printInfos(RandomAccessFile file) throws IOException{
        int index = 0;
        int[] signature = ARRAY_SIGNATURE[index];
        int off = 0;
        int length = 0;
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
                    if(signature == SIGNATURE_WINDOWS){
                        os = "Windows";
                        off = CPU_OFFSET_WINDOWS;
                        length = CPU_LENGTH_WINDOWS;
                    }
                    else if(signature == SIGNATURE_MAC){
                        os = "Mac";
                        off = CPU_OFFSET_MAC;
                        length = CPU_LENGTH_MAC;
                    }
                    else if (signature == SIGNATURE_LINUX){
                        os = "Linux";
                        off = CPU_OFFSET_LINUX;
                        length = CPU_LENGTH_LINUX;
                    }
                    else{
                        System.out.println("Pas trouver");}
                    count--;
                }
                count++;
            }
            else{
                count = 0;
            }

            try{
                if (value == -1){
                    signature = ARRAY_SIGNATURE[index++];
                    i=-1;
                }
            }catch (IndexOutOfBoundsException e){
                index--;
            }

            //trouver l'architecture
            if (!os.equals("")){
                //System.out.println("\u001B[52m" + response);
                for (int offset = off ; offset < (off + length); offset++){
                    file.seek(response + offset);
                    cpu += Integer.toHexString(file.read());
                }
                cpu = switch (cpu) {
                    case "6486", "3e", "7" -> "AMD64";
                    case "64aa", "b7", "12" -> "ARM64";
                    default -> cpu;
                };
            }
            i++;
        }
        System.out.println("OS: " + os);
        System.out.println("Machine: " + cpu);
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