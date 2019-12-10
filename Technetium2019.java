import java.io.*; 
import java.util.*; 

class Solution {
    private int V;
    private int N;
    private int M;
    private byte[][] A;
    private byte[][] goodPlaces;
    Boolean history(int i, int j){
        if(i == 0 && j == 0)
            return true;
        if(i > 0 && 1 == goodPlaces[i-1][j])
            return true;
        if(j > 0 && 1 == goodPlaces[i][j-1])
            return true;
        return false;
    }
    String calc(){
        String res = ""+ A[0][0];
        goodPlaces[0][0] = 1;
        int maxInDiagonal = 0;
        for(int i = 1; i < N; i++){
            maxInDiagonal = 0;
            int j = 0;
            int i2 = i;
            while(  j < M && i2 >=0 ) {
              if(history( i2,  j) && A[i2][j] > maxInDiagonal)
                 maxInDiagonal = A[i2][j];
               i2--;
               j++;
            } 
            i2 = i;
            j = 0;
           while(  j < M && i2 >=0 ) {
              if(history( i2,  j) && A[i2][j] == maxInDiagonal)
                 goodPlaces[i2][j] = 1;
              i2--;
              j++;
            }
            res += maxInDiagonal;
        }
        for(int j = 1; j < M; j++){
            int i = N-1;
            int j2 = j;
            maxInDiagonal = 0;
            while( i>= 0 &&  j2 < M ) {
               if(history( i,  j2) && A[i][j2] > maxInDiagonal)
                 maxInDiagonal = A[i][j2];
                i--;
                j2++;
            } 
            i = N-1;
            j2 = j;
            while( i>= 0 &&  j2 < M ) {
               if(history( i,  j2) && A[i][j2] == maxInDiagonal)
                 goodPlaces[i][j2] = 1;
                i--;
                j2++;
            }
            res += maxInDiagonal;
        }
        return res;
    }
    public String solution(int[][] _A) {
        N = _A.length;
        M = _A[0].length;
        V = N * M;
        A = new byte[N][M]; 
        goodPlaces = new byte [N][M];
        for(int i = 0; i < N; i++) {
            for(int j = 0; j < M; j++) {
                A[i][j] = (byte)_A[i][j];
            }
        }
        return calc();
    }
}