import java.util.*;
import java.lang.Math;
import java.lang.StringBuilder;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class CheckerHW1 {
    public static void main(String[] args) {
        try {

                StringBuilder sb = new StringBuilder();

                List<String> logList = Files.readAllLines(Paths.get("log.txt"));
                int total = Integer.parseInt(logList.get(1));
                int solFound = Integer.parseInt(logList.get(4));
                int correct = Integer.parseInt(logList.get(6));
                int incorrect = Integer.parseInt(logList.get(8));
                int notFound = Integer.parseInt(logList.get(11));

                total++;

                List<String> resList = Files.readAllLines(Paths.get("output.txt"));
                String result = resList.get(0);

                if (result.equals("FAIL")) {
                    notFound++;
                    sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                    Files.write(Paths.get("log.txt"), sb.toString().getBytes());
                    
                    System.out.println("[*] Output:");
                    for (String s : resList) {
                        System.out.println(s);
                    }
                    System.out.println("-----------------------------");


                    List<String> inFile = Files.readAllLines(Paths.get("input.txt"));
                    StringBuilder sb2 = new StringBuilder();
                    for (String s : inFile) {
                        sb2.append(s);
                        sb2.append("\n");
                    }


                    Files.write(Paths.get("./fail_cases/failMap" + System.currentTimeMillis() + ".txt"), sb2.toString().getBytes());
                    System.exit(1);
                }

                solFound++;

                int size = resList.get(1).length();
                char[][] map = new char[size][size];
                for (int i = 0; i < size; i++) {
                    char[] line = resList.get(1 + i).toCharArray();
                    map[i] = line;
                }


                for (int i = 0; i < size; i++) {
                    for (int j = 0; j < size; j ++) {
                        if (map[i][j] == '0' || map[i][j] == '2') {
                            continue;
                        }


                        int idx = j + 1;
                        while (idx < size && map[i][idx] != '2') {
                            if (map[i][idx] == '1') {
                                incorrect++;
                                sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                                Files.write(Paths.get("log.txt"), sb.toString().getBytes());

                                System.out.println("[!] ERROR: ANSWER INCORRECT");                                  
                                System.out.println("[*] Output:");
                                for (String s : resList) {
                                    System.out.println(s);
                                }
                                System.out.println("-----------------------------");

                                System.exit(1);
                            }
                            idx++;
                        }

                        idx = 1;
                        while ( j + idx < size && i + idx < size && map[i + idx][j + idx] != '2') {
                            if (map[i + idx][j + idx] == '1') {
                                incorrect++;
                                sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                                Files.write(Paths.get("log.txt"), sb.toString().getBytes());

                                System.out.println("[!] ERROR: ANSWER INCORRECT");                                  
                                System.out.println("[*] Output:");
                                for (String s : resList) {
                                    System.out.println(s);
                                }
                                System.out.println("-----------------------------");

                                System.exit(1);
                            }
                            idx++;
                        }

                        idx = 1;
                        while ( i + idx < size && map[i + idx][j] != '2') {
                            if (map[i + idx][j] == '1') {
                                incorrect++;
                                sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                                Files.write(Paths.get("log.txt"), sb.toString().getBytes());

                                System.out.println("[!] ERROR: ANSWER INCORRECT");                            
                                System.out.println("[*] Output:");
                                for (String s : resList) {
                                    System.out.println(s);
                                }
                                System.out.println("-----------------------------");

                                System.exit(1);
                            }
                            idx++;
                        }
                        idx = 1;
                        while ( i + idx < size && j - idx >= 0 && map[i + idx][j - idx] != '2') {
                            if (map[i + idx][j - idx] == '1') {
                                incorrect++;
                                sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                                Files.write(Paths.get("log.txt"), sb.toString().getBytes());
                                

                                System.out.println("[!] ERROR: ANSWER INCORRECT");  
                                System.out.println("[*] Output:");
                                for (String s : resList) {
                                    System.out.println(s);
                                }
                                System.out.println("-----------------------------");

                                System.exit(1);
                            }
                            idx++;
                        }
                    }
                }

                correct++;
                sb.append("Total:\n" + total + "\n\n" + "Solution found:\n" + solFound + "\n" + "Correct:\n" + correct + "\n" + "Incorrect:\n" + incorrect + "\n\n" + "Solution not found:\n" + notFound);  
                Files.write(Paths.get("log.txt"), sb.toString().getBytes());
                System.out.println("[*] Output:");
                for (String s : resList) {
                    System.out.println(s);
                }
                System.out.println("-----------------------------");

        }
        catch (IOException e) {
            System.out.println(" - File Error:");
            System.out.println(e);
            System.exit(1);
        }
    }
}