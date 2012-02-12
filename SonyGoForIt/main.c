/***
 *
 * 120212 19:22
 *
 * Sony GO FOR IT 問題2(http://www.sony.co.jp/SonyInfo/Jobs/newgrads/sus/q02.html)
 * 
 * 解法のポイント: 
 *   実数の階乗 fact(n) = gamma (n+1) を利用
 *   近似式は 
 *
 *   log(gamma(n)) = log (n) * (n - 0.5) - n + log(2*PI)*0.5 +
 *                   sum(i=1...){ B(2n) / (2i * (2i-1) * x^(2i-1) };
 *
 *   ここで、B(i)はBernouilli数であり、
 *
 *   B(0) = 1
 *   B(n) = -1/(n+1) * sum(i=0:n-1){Combination(n+1,i) * B(i)}
 *
 *   となる。
 *   今回は i = 20 までで近似値を取っている
 *
 *   ただし、 この近似は n >> 0 である必要があるため
 *   gamma (n+1) = n * gamma (n) 
 *   を利用して、n が十分大きくない場合は (今回はn < 5 の時)
 *   gamma (n) = gamma (n+1)/n  (n != 0)
 *   として、 n の値を大きくしてから近似値を計算している
 ***/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

double B[21];

long long int combi (int n, int r)
{
  int i;
  long long int sum = 1;
  assert (n >= r);

  for (i = 0; i < r; i++) {
    sum *= n-i;
  }
  for (i = r; i > 0; i--) {
    sum /= i;
  }
  return sum;
}

double Bernouilli (int n)
{

  int i;
  double sum = 0;
  long long int  C;

  assert (n <= 20);
  if (B[n] != 0) { return B[n]; }

  if (n == 0) { return 1; }
  
  for (i = 0; i < n; i++) {
    C = combi(n+1, i);
    sum += C * Bernouilli (i);
  }
  B[n] = -sum / (n+1);
  return B[n];
}

double log_gamma (double n)
{
  double sum, w, temp = 0;
  int i;

  w = 1/(n*n);
  sum = log (n) * (n - 0.5) - n + log(2*M_PI)*0.5;

  for (i = 18; i >= 2; i-=2) {
    temp = (temp + B[i]/(i*(i-1)))*w;
  }
  sum = sum + temp*n;

  return sum;
}

double gamma (double n)
{
  // n >> 0 である必要がある為、ある程度大きくする
  // gamma(x+1) == x * gamma (x) より
  if (n == 0) {
    return 1;
  }
  if (n <= 5) {
    return gamma (n+1) / n;
  }
  return exp (log_gamma (n));
}

double fact (double n)
{
  return gamma (n+1);
}

int main (int argc, char *argv[])
{
  int i = 0;
  double n, ans;

  if (argc < 2) {
    fprintf (stderr, "Please input a Real Number (ex.: ./main.out 1.5)\n");
    exit (0);
  }
  
  for (i = 0; i <= 20; i++) {
    B[i] = 0;
  }

  for (i = 0; i <= 20; i++) {
    double a = Bernouilli (i);
  }

  for (i = 1; i < argc; i++) {
    n = atof (argv[i]);
    ans = fact (n);
    printf ("fact(%f): %.8f\n", n, ans);
  }

  return 0;
}

