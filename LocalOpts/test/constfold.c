int compute() {
    int test = 5;
    int result = test;
    int a = 2;
    int b = 3;
    int c = 4 + a + b;

    result += a;
    result += b;
    result *= c;
    result /= 2;
    result += test;
    return result;
}
