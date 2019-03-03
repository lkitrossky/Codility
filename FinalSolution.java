// https://app.codility.com/programmers/challenges/yttrium2019/
// https://app.codility.com/cert/view/certVYAXVR-B8ZAWXKW55MRCVMG/
// https://app.codility.com/cert/view/certN4TQE8-ZYQRT2DZ5ZBGAGD2/      // cleaner version
// Levi Kitrossky was awarded Codility Golden Award for the Yttrium 2019 Challenge
// https://www.onlinegdb.com/edit/H1BKRrPr4
import java.util.Arrays;

//public class Solution
/*

		Main ideas
	General
We cut some fragment but count out of the fragment, so the first idea is to count total amount of symbols per symbol
And then to count all symbols that their amount is > 0. Thus we obtain general amount of unique symbols (howMany in the code).
	Caterpillar
The amount of symbols outside of the cut fragment depends on the left and right border
It is a function depending of two variables. 
Both dependencies are monotonous: from left not decreasing, from right - not increasing.
All this hints on the caterpillar method. Move right border if too big, move left border if too small. 
It is convenient to take left as the first cut out symbol, and right as the symbol after the last cut. 
If the initial array has size SIZE, left < SIZE, right <=SIZE.  We need not check left < right, because if they are equal it means that nothing is uncut and 
the possibility of such case checked before cycles
Special cases first:
Total unique amount howMany < K desired, impossible
Desired unique amount K is 0 - cut all, return length
Desired amount K equals unique amount present howMany, cut nothing, return 0

*/
class Solution
{
	public static final int ABC_SIZE = 'z' - 'a' + 1;
	public static String _S;
	public static int SIZE;
	private static int [] amountOfChar = new int [ABC_SIZE]; //amountOfChar[i] - the first appearance of character i+'a' moving from left 
	private static int howMany = 0; //how many unique characters in the original string 	
	private static void initiate(String S1, int K){
		_S = S1;
		SIZE = _S.length();
		amountOfChar = new int [ABC_SIZE]; 
	    howMany = 0; 
		for(int i = 0; i < ABC_SIZE;i++){
			amountOfChar [i] = 0;
		}
		for(int i = 0; i < SIZE;i++){
			char c = _S.charAt(i);
			amountOfChar[c-'a']++;
		}
		for(int i = 0; i < ABC_SIZE;i++){
			if(amountOfChar [i] > 0)
				howMany++;
		}
	}
	public static int solution(String S, int K)	{
	     initiate(S, K);
		if(howMany < K)
			return -1;
		if(howMany == K)
			return 0;
		if(0 == K )
			return S.length();
		int res = -1;
		int howManyUncutCurrent = howMany;
		int left = 0;
		int right = 1;
		while(right <= SIZE){  
			char cr = _S.charAt(right - 1); //we moved right, so char at position right-1 is cut
			amountOfChar[cr-'a']--;
			if(0 == amountOfChar[cr-'a'])  //c is not more uncut, so the amount of uncut decreased
				howManyUncutCurrent--;
			while(howManyUncutCurrent <= K && left < SIZE){ 
				if(howManyUncutCurrent == K){ // now after right++, check if we are OK. 
					if(-1 == res)
						res = right-left;
					else
						res = Math.min(res, right-left);
				}
				left++; //OK or too less - move the left
				if(left >= SIZE)
					break;
				char cl = _S.charAt(left-1);
				if(0 == amountOfChar[cl-'a']){  //and check if uncut diminished	
					howManyUncutCurrent++;
				}	
				amountOfChar[cl-'a']++;
			}	
			right++;
		}
		return res;
	}
}

/// Tests and driver

public class Main {
	public static void test(String S, int K, int expected)
	{
		Solution s = new Solution();
		int res = s.solution(S, K);
		System.out.print(S);
		System.out.print(" K = ");
		System.out.print(K);
		System.out.print(" Expected: ");
		System.out.print(expected);
		System.out.print(" Calculated: ");
		System.out.print(res);
		if (res != expected)
		{
			System.out.print(" Error!\n");
		} else {
		    System.out.print(" OK\n");
		}
	}
	public static void main(String[] args)
	{	
		test("aaa", 2, -1);
		test("baaa", 1, 1);
		test("zaaa", 1, 1);
		test("aabcabc", 1, 5);
		test("aabaa", 1, 1);
		test("aabaac", 2, 1); 
		test("aaaa", 1, 0);
		test("aaaab", 1, 1);
		test("aaaab", 2, 0);
		test("aaaab", 3, -1);
		test("aabaacbca", 2, 3);
	}	
}

