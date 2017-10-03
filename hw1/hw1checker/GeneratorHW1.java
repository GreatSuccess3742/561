import java.lang.Math;
import java.util.Random;
import java.lang.StringBuilder;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;

public class GeneratorHW1 {
    private static Random rand = new Random();
    private static String[] types = new String[]{"DFS", "BFS", "SA"};
    private static int[] sizeLimits = new int[]{10, 8, 20};
    private static int lizards;
    private static int trees;
    private static char[][] map;
    public static void main(String[] args) {
        try {
                int randNum = rand.nextInt(3);
                String type = types[randNum];
                
                int limit = rand.nextInt(sizeLimits[randNum]) + 1;
                // int caseNum = rand.nextInt(2);

                trees = rand.nextInt(limit * limit + 1);
                int MaxLizards = Math.min(limit + trees, limit * limit - trees);
                MaxLizards = Math.min(MaxLizards, limit * limit / 4);
                if (MaxLizards == 0) {
                    MaxLizards = 1;
                }
                lizards = rand.nextInt(MaxLizards) + 1; 

                map = new char[limit][limit];
                for (int i = 0; i < limit; i ++) {
                    for (int j = 0; j < limit; j++) {
                        map[i][j] = '0';
                    }
                }

                while (trees != 0) {
                    int r = rand.nextInt(limit);
                    int c = rand.nextInt(limit);
                    while (map[r][c] != '0') {
                        r = rand.nextInt(limit);
                        c = rand.nextInt(limit);
                    }
                    map[r][c] = '2';
                    trees--;
                }

                StringBuilder sb = new StringBuilder();
                sb.append(type);
                sb.append("\n");
                sb.append(limit);
                sb.append("\n");
                sb.append(lizards);
                sb.append("\n");

                for (int i = 0; i < map.length; i++) {
                    for (int j = 0; j < map.length; j++) {
                        sb.append(map[i][j]);
                    }
                    sb.append("\n");
                }
                Files.write(Paths.get("input.txt"), sb.toString().getBytes());
                System.out.println("[*] Random map:");
                System.out.println(sb.toString());
        }
        catch (IOException e) {
            System.out.println(" - File Error:");
            System.out.println(e);
            System.exit(1);
        }
    }
}