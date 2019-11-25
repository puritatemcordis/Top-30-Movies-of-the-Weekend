#include <iostream>
#include <math.h>
#include <assert.h>

using namespace std;

/* precondition: a>=b>=0 */
/* postcondition: return d=gcd(a,b) */
int EuclidAlgGCD (int a, int b){
  while (a!=0 && b!=0)
   {
      a = a % b;
      if (a!=0)
         b = b % a;
   }
   if (a==0)
      return b;
   if (b==0)
      return a;
};

/* precondition: a>=b>=0 */
/* postcondition: return d=gcd(a,b), s and t are set so that d=sa+tb */
int ExtendedEuclidAlgGCD (int a, int b, int *s, int *t){
  if (a ==0){
    *s = 0;
    *t = 1;
    return b;
  }

  int x, y; // stores results of recursive call
  int gcd = ExtendedEuclidAlgGCD(b%a, a, &x, &y);

  // Update s and t using results of recursive

  *s = y - (b/a) * x;
  *t = x;

  return gcd;
};

/* precondition: n is greater than 1,a can be negative
postcondition: return a mod n (as defined in class)
               a mod n = r if and only if a = nk+r, 0 =< r < n (note that
               r needs to be non-negative).
*/
int mod (int a, int n){
  int q, r, p; // quotient, remainder, product

  if (a>0){
    q = a/n;
    p = q*n;
    r = a-p;
    return r;
  } else {
    a = a*-1;
    q = a/n;
    p = q*n;
    r = a-p;
    return n - r;
  }
};

/* n>1, a is nonnegative */
/* a<=n */
/* a and n are relative prime to each other */
/* return s such that a*s mod n is 1 */
int inverse (int a, int n)
{
  int s, t;
  int d = ExtendedEuclidAlgGCD (n, a, &s, &t);

  if (d==1)
  {
    return (mod (t, n)); // t might be negative, use mod() to reduce to
                         // an integer between 0 and n-1
  }
  else
  {
    cout <<"a and n are not relatively prime!\n";
  }
};

/* return true if n is prime
   return false if n is not prime
   Precondition: n>1
*/
bool IsPrime (int n){
  for(int i=2; i<=n/2; i++){
    int remainder = mod(n, i);
    if(remainder == 0)
      return false;
  }
  return true;
}

/* return an integer that is relatively prime with n, and greater than 1
   i.e., the gcd of the returned int and n is 1
   Note: Although gcd(n,n-1)=1, we don't want to return n-1
*/
int RelativelyPrime (int n){
  for (int i=2; i < n/2; i++){
    int gcd = EuclidAlgGCD(n, i);
    if (gcd == 1)
      return i;
  }
}

// Returns M^d mod PQ
int Encode (int M, int d, int PQ){
  int product = 1;
  // cout << "M is " << M << endl;
  for(int i=0; i<d;i++)
    product = (product * M) % PQ;

  return product;
}

// Returns C^e mod PQ
int Decode (int C, int e, int PQ){
  int product = 1;
  // cout << "C is " << C << endl;
  for(int i=0; i<e;i++)
    product = (product * C) % PQ;

  return product;
}

int main(){
  // two random prime numbers
  const int p = 23;
  const int q = 29;

  // first part of public key
  int pq = p*q;                         // n
  // cout << "pq is " << pq << endl;
  int phi = (p-1)*(q-1);               //phi
  // cout << "the phi is " << phi << endl;

  // STEP 2
  int e = RelativelyPrime(phi);        //e
  // cout << "e is " << e << endl;
  // STEP 3 FUNCTION DOES NOT RETURN CORRECT VALUE
  // int d = inverse(phi, e);             //gcd(phi, e)
  // cout << "D using inverse is " << d << endl;

  int k = 2;
  while ((1+k*phi)%e != 0){
    k++;
  }
  // cout << "The k is " << k << endl;

  int d = (1 + (k*phi))/e;
  // cout << "The d is " << d << endl;

  // STEP 6
  int M;
  // M is an integer that is smaller than pq
  cout << "Enter an integer that is smaller than " << pq << "(This is your message): ";
  cin >> M;

  int C = Encode(M, e, pq);
  cout << "The encoded message is " << C << endl;
  int DC = Decode(C, d, pq);
  cout << "The original message is " << DC << endl;
  assert(M == DC);

  return 0;
}
