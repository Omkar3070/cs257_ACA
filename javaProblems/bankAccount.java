public abstract class bankAccount{

  double balance = 1000;
  String name;
  int acountNumber;
  double overDraft = 150;

public double getAccountBalance(){
  return balance;
}

public double getAvailableBalance(){
  return (balance + overDraft);
}

public double transferMoney(int newAccountNum, double amount){
  balance = balance - amount;
  return balance;
}

public double depositMoney(int amount){
  balance = balance + amount;
  return balance;
}

public abstract double withdrawMoney();
}
