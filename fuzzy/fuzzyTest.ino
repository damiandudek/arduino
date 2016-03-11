/*
  need to fuzzy library
  http://www.zerokol.com/2012/09/arduinofuzzy-fuzzy-library-for-arduino.html
*/

#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

Fuzzy* fuzzyCold = new Fuzzy();
Fuzzy* fuzzyComfortable = new Fuzzy();
Fuzzy* fuzzyHot = new Fuzzy();

FuzzySet* cold = new FuzzySet(0, 0, 10, 18);
FuzzySet* good = new FuzzySet(16, 22, 26, 30);
FuzzySet* hot = new FuzzySet(26, 30, 34, 34);

FuzzySet* ac_on = new FuzzySet(0, 1, 1, 1);
FuzzySet* ac_off = new FuzzySet(0, 0, 0, 0);

FuzzySet* heater_on = new FuzzySet(0, 1, 1, 1);
FuzzySet* heater_off = new FuzzySet(0, 0, 0, 0);

void setup(){
  Serial.begin(9600);
  Serial.println(); 
  
  addFuzzyInputs(fuzzyCold);
  addFuzzyOutputs(fuzzyCold);  
  
  addFuzzyInputs(fuzzyComfortable);
  addFuzzyOutputs(fuzzyComfortable);

  addFuzzyInputs(fuzzyHot);
  addFuzzyOutputs(fuzzyHot);   
  
  rulesForCold(fuzzyCold);
  rulesForComfortable(fuzzyComfortable);
  rulesForHot(fuzzyHot);
}

void addFuzzyInputs(Fuzzy *fuzzy){
  // FuzzyInput
  FuzzyInput* temperature = new FuzzyInput(1);
  temperature->addFuzzySet(cold);
  temperature->addFuzzySet(good);
  temperature->addFuzzySet(hot);
  fuzzy->addFuzzyInput(temperature);
}

void addFuzzyOutputs(Fuzzy *fuzzy){
  // FuzzyOutput
  // Air Conditioner
  FuzzyOutput* ac = new FuzzyOutput(1);
  ac->addFuzzySet(ac_off);
  ac->addFuzzySet(ac_on);
  fuzzy->addFuzzyOutput(ac);
  
  // FuzzyOutput
  // Heater
  FuzzyOutput* heater = new FuzzyOutput(2);
  heater->addFuzzySet(heater_off);
  heater->addFuzzySet(heater_on);
  fuzzy->addFuzzyOutput(heater);
}

void loop(){
  while (Serial.available() > 0) {  
    char inChar = (char)Serial.read();
    if(inChar == 'q'){
      fuzzyTest(fuzzyCold);
    }
    if(inChar == 'w'){
      fuzzyTest(fuzzyComfortable);
    }
    if(inChar == 'e'){
      fuzzyTest(fuzzyHot);
    }
  }
}


int temp = 0;
int stopi = 0;
float output1;
float output2;


void fuzzyTest(Fuzzy *fuzzy){
  Serial.println("Start new fuzzy test");
  Serial.println("Temp1     cold     OK     hot     AC     H");
  
  stopi = 0;
  for(temp = 6; temp <= 33; temp++){ 
   if(temp > 32){
     stopi = 1;
   }
   
   if(stopi != 1){
     fuzzy->setInput(1, temp);

    if(temp < 10)
      Serial.print(" ");
      
      Serial.print(" ");
      Serial.print(temp);
      Serial.print("      ");
      Serial.print(cold->getPertinence());
      Serial.print("      ");
      Serial.print(good->getPertinence());
      Serial.print("      ");
      Serial.print(hot->getPertinence());
      Serial.print("      ");
    
      fuzzy->fuzzify();
    
      output1 = fuzzy->defuzzify(1);
      output2 = fuzzy->defuzzify(2);
      
      Serial.print("    ");
      Serial.print(output1);
      Serial.print("    ");
      Serial.print(output2);
      Serial.println();
     }
  }
}

 
void rulesForCold(Fuzzy *fuzzy){  

  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent();
  ifTemperatureCold->joinSingle(cold);
  
  FuzzyRuleConsequent* thenTemperatureCold = new FuzzyRuleConsequent();
  thenTemperatureCold->addOutput(heater_off);
  thenTemperatureCold->addOutput(ac_off);
  
  FuzzyRule* fuzzyRule2 = new FuzzyRule(1, ifTemperatureCold, thenTemperatureCold);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  
  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureGood = new FuzzyRuleAntecedent();
  ifTemperatureGood->joinSingle(good);
  FuzzyRuleConsequent* thenTemperatureGood = new FuzzyRuleConsequent();
  thenTemperatureGood->addOutput(heater_off);
  thenTemperatureGood->addOutput(ac_on);
  FuzzyRule* fuzzyRule3 = new FuzzyRule(2, ifTemperatureGood, thenTemperatureGood);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
    // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent();
  ifTemperatureHot->joinSingle(hot);
  FuzzyRuleConsequent* thenTemperatureHot = new FuzzyRuleConsequent();
  thenTemperatureHot->addOutput(heater_off);
  thenTemperatureHot->addOutput(ac_on);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(3, ifTemperatureHot, thenTemperatureHot);
  fuzzy->addFuzzyRule(fuzzyRule3);

}

void rulesForComfortable(Fuzzy *fuzzy){  

  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent();
  ifTemperatureCold->joinSingle(cold);
  
  FuzzyRuleConsequent* thenTemperatureCold = new FuzzyRuleConsequent();
  thenTemperatureCold->addOutput(heater_on);
  thenTemperatureCold->addOutput(ac_off);
  
  FuzzyRule* fuzzyRule2 = new FuzzyRule(1, ifTemperatureCold, thenTemperatureCold);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  
  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureGood = new FuzzyRuleAntecedent();
  ifTemperatureGood->joinSingle(good);
  FuzzyRuleConsequent* thenTemperatureGood = new FuzzyRuleConsequent();
  thenTemperatureGood->addOutput(heater_off);
  thenTemperatureGood->addOutput(ac_off);
  FuzzyRule* fuzzyRule3 = new FuzzyRule(2, ifTemperatureGood, thenTemperatureGood);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
    // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent();
  ifTemperatureHot->joinSingle(hot);
  FuzzyRuleConsequent* thenTemperatureHot = new FuzzyRuleConsequent();
  thenTemperatureHot->addOutput(heater_off);
  thenTemperatureHot->addOutput(ac_on);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(3, ifTemperatureHot, thenTemperatureHot);
  fuzzy->addFuzzyRule(fuzzyRule43);

}

void rulesForHot(Fuzzy *fuzzy){

  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureCold = new FuzzyRuleAntecedent();
  ifTemperatureCold->joinSingle(cold);
  
  FuzzyRuleConsequent* thenTemperatureCold = new FuzzyRuleConsequent();
  thenTemperatureCold->addOutput(heater_on);
  thenTemperatureCold->addOutput(ac_off);
  
  FuzzyRule* fuzzyRule2 = new FuzzyRule(1, ifTemperatureCold, thenTemperatureCold);
  fuzzy->addFuzzyRule(fuzzyRule1);
  
  
  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureGood = new FuzzyRuleAntecedent();
  ifTemperatureGood->joinSingle(good);
  FuzzyRuleConsequent* thenTemperatureGood = new FuzzyRuleConsequent();
  thenTemperatureGood->addOutput(heater_on);
  thenTemperatureGood->addOutput(ac_off);
  FuzzyRule* fuzzyRule3 = new FuzzyRule(2, ifTemperatureGood, thenTemperatureGood);
  fuzzy->addFuzzyRule(fuzzyRule2);

  
  // Building FuzzyRule
  FuzzyRuleAntecedent* ifTemperatureHot = new FuzzyRuleAntecedent();
  ifTemperatureHot->joinSingle(hot);
  FuzzyRuleConsequent* thenTemperatureHot = new FuzzyRuleConsequent();
  thenTemperatureHot->addOutput(heater_off);
  thenTemperatureHot->addOutput(ac_off);
  FuzzyRule* fuzzyRule4 = new FuzzyRule(3, ifTemperatureHot, thenTemperatureHot);
  fuzzy->addFuzzyRule(fuzzyRule3); 
}
