int compute (int a, int b)
{
  int add0 = a + 0;
  int add1 = 0 + a;
  int sub0 = a - 0;
  int sub1 = a - a;
  int result = (a/a);

  result *= (b/b);
  result += (b-b);
  result /= result;
  result -= result;
 
  return result;
}
