
  
class input {

public:
  
  input( char *chosenDevice);

  ~input();

  int returnLastKey(void);

  void waitUntilChange(void);

  void waitUntilKey(int key);

private:

  pthread_t cm;

  int newOutput;
};
  



  
