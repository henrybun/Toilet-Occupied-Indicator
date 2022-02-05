
void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  String input = "";
  int output[10];
  for (int i = 0; i < 10; i++) {
    input.concat(random(0, 100));
    input.concat("*");
  }
  Serial.println(input);
  int start = 0, end = 0, outputIndex = 0;
  for(int i = 0;i < input.length();i++){
    if(input.charAt(i) == '*'){
      String temp = input.substring(start,end);
      output[outputIndex] = temp.toInt();
      start = end + 1;
      outputIndex++;
    }
    end++;
  }
  for(int i = 0;i < 10;i++){
    Serial.print(output[i]);
    Serial.print("\n");
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
