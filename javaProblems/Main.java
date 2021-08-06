import java.util.Scanner;
import java.util.Arrays;

public class Main{

  //The following shows what the numbers stored in the array
  // stand for
  // 1 - Player ships
  // 2 - Computer ships
  // 3 - Player sunk a computer ship
  // 4 - Player sank their own ship
  // 5 - Player missed
  // 6 - Computer sunk a player ship
  // 7 - Computer sank their own ship
  // 8 - Computer missed

  //The following shows what the symbols displayed stand for
  // (@) - Player ships
  // (-) - Player missed his attack
  // (!) - Player sunk a Computer ship
  // (x) - Player sank one of his own ship

  //This is to keep count of the ships remaining.
  public static int pShips = 5;
  public static int cShips = 5;

  //Creates the Ocean map for the ships to battle.
  //The Ocean map is made using a 2D array. This Function
  //displays the map on the screen
  public static void map(int[][] array){
    System.out.println(" ");
    //These numbers are used to display the coordinates on
    //the map to deploy and attack ships
    System.out.println("  0123456789  ");
    for(int row = 0; row < array.length; row++){
      System.out.print(row + "|");
      for(int col = 0; col < array[row].length; col++){
        //By default the array is filled with zeros.
        //Everywhere there is a zero is an empty space
        //on the map
        if(array[row][col] == 0){
          System.out.print(" ");
        }
        //Shows the Player's ships on the map
        else{
          if(array[row][col] == 1){
            System.out.print("@");
          }
          //Hides the Computer's ships
          else if(array[row][col] == 2 || array[row][col] == 8){
            System.out.print(" ");
          }
          //When the Computer's ship is sunk, the symbol
          //below is displayed.
          else if(array[row][col] == 3 || array[row][col] == 7){
            System.out.print("!");
          }
          //When the Player's shp is sunk, the symbol
          //below is displayed.
          else if(array[row][col] == 4 || array[row][col] == 6){
            System.out.print("x");
          }
          //When the player or Computer misses, the symbol
          //below is displayed.
          else if(array[row][col] == 5){
            System.out.print("-");
          }
        }
      }
      System.out.println("|" + row);
    }
    System.out.println("  0123456789\n  ");
    System.out.println("Your ships: " + pShips + " | " + "Computer ships: " + cShips);
    System.out.println("------------------------------------------------------------");
  }

  //Deploy players ships
  public static void playerDeploy(int[][] array1){
    Scanner input = new Scanner(System.in);
    System.out.println("\nDeploy your ships");
    for(int i = 1; i <= 5; i++){
      System.out.print("\nEnter X coordinate for your " + i + ". ship: ");
      int x = input.nextInt();
      System.out.print("Enter Y coordinate for your " + i + ". ship: ");
      int y = input.nextInt();

      //if the player trys to put the ship outside the grid or
      //tries to place more than 1 ship on the same location,
      //reprompt them
      while((x > 9 || y > 9) || (array1[x][y] == 1)){
        if(x > 9 || y > 9){
          System.out.println("Can't place ships outside the sea");
        }else if(array1[x][y] == 1){
          System.out.println("Can't place 2 or more ships on the same location");
        }
        System.out.print("Enter X coordinate for your " + i + ". ship: ");
        x = input.nextInt();
        System.out.print("Enter Y coordinate for your " + i + ". ship: ");
        y = input.nextInt();
      }
      array1[x][y] = 1;
    }

  }

  //Deploy Computer's ships
  public static void comDeploy(int array2[][]){
    System.out.println("\nComputer is deploying ships");
    //The loop is used to deploy five ships for the Computer.
    for(int z = 1; z <= 5; z++){
      int x = (int)Math.round(Math.random()*9);
      int y = (int)Math.round(Math.random()*9);
      //If the Computer tries to place more than 1 ship on the same
      // location re-generate random numbers to make sure every ship
      //is on a distinct location
      while(array2[x][y] == 2 || array2[x][y] == 1){
        x = (int)Math.round(Math.random()*9);
        y = (int)Math.round(Math.random()*9);
      }
      array2[x][y] = 2;
      System.out.println("ship Deployed");
    }
  }

  //The method enables the Player to attack the Computers ships
  //by sinking them.
  public static void playerAttack(int array3[][]){
    Scanner input = new Scanner(System.in);
    System.out.println("\nYour Turn");
    System.out.print("Enter the X coordinate for you attack: ");
    int x = input.nextInt();
    System.out.print("Enter the Y coordinate for your attack: ");
    int y = input.nextInt();
    //if the coordinates for attck are outside the grid then reprompt
    //the user to enter valid coordinates.
    while(x > 9 || y > 9){
      System.out.println("Please enter a valid guess");
      System.out.print("Enter the X coordinate for you attack: ");
      x = input.nextInt();
      System.out.print("Enter the Y coordinate for your attack: ");
      y = input.nextInt();
    }
    //if the guess is right then sink the ship and reduce the
    //number of Computer's ships.
    if(array3[x][y] == 2){
      System.out.println("Boom! You sunk the ship");
      cShips--;
      array3[x][y] = 3;
    }
    //if the Player tries to attck an already attacked location,
    //re-prompt them.
    else if((array3[x][y] == 3) || (array3[x][y] == 4)){
      System.out.println("This ship is already sunk, Try again");
      System.out.print("Enter the X coordinate for you attack: ");
      x = input.nextInt();
      System.out.print("Enter the Y coordinate for your attack: ");
      y = input.nextInt();
    }
    //if the Player guesses the location of one of his own ships,
    //then sink the ship and reduce the number of Player ships.
    else if(array3[x][y] == 1){
      System.out.println("Oh no, you sunk one of your own the ships");
      pShips--;
      array3[x][y] = 4;
    }
    //Print message when Player misses.
    else if((array3[x][y] != 3) && (array3[x][y] != 4)){
      System.out.println("Sorry, you missed");
      array3[x][y] = 5;
    }
  }

  //This method randomly generate coordinates to attack other ships.
  public static void comAttack(int array4[][]){
    System.out.println("\nComputer's Turn");
    int x = (int)Math.round(Math.random()*9);
    int y = (int)Math.round(Math.random()*9);
    //Reduce the number of player ships when one of player's ships are sunk.
    if(array4[x][y] == 1){
      System.out.println("The Computer sank one of your ships");
      pShips--;
      array4[x][y] = 6;
    }
     //Reduce the number of Computer ships when it sinks one of its own
     //ships.
     else if(array4[x][y] == 2){
      System.out.println("The Computer sank one of its own ships");
      cShips--;
      array4[x][y] = 7;
    }
    //The code below re-generates random numbers when it tries to attack
    //an already attacked location.
    else if((array4[x][y] == 8) || (array4[x][y] == 6) || (array4[x][y] == 7)){
      while(array4[x][y] == 8){
        x = (int)Math.round(Math.random()*9);
        y = (int)Math.round(Math.random()*9);
      }
    }
    //Print message when Computer misses.
    else if((array4[x][y] != 6) && (array4[x][y] != 7)){
      System.out.println("Computer missed");
      array4[x][y] = 8;
    }
  }

  public static void main(String[] args){
    System.out.println("**** Welcome to Battle Ships game ****");
    System.out.println("Right now, the sea is empty");

    //Creates and initialises the array.
    int[][] ocean = new int[10][10];

    //Deploy players ships.
    playerDeploy(ocean);
    comDeploy(ocean);

    //Keep on attacking until the Player or the Computer has
    //no ships left.
    while((pShips != 0) && (cShips != 0)){
      playerAttack(ocean);
      comAttack(ocean);
      map(ocean);
    }
    //Print the message depending on who lost all their ships.
    if(pShips == 0){
      System.out.println("Oh no, You Lost");
    } else if(cShips == 0){
      System.out.println("Hooray!, You won the game");
    }

  }
}
