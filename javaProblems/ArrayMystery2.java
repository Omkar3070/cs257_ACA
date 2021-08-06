import java.util.*;

public class ArrayMystery2{
  public static void mystery2(ArrayList<Integer> list) {
    for (int i = list.size() - 1; i >= 0; i--) {
        if (i % 2 == 0) {
            list.add(list.get(i));
        } else {
            list.add(0, list.get(i));
        }
    }
    System.out.println(list);
}

  public static void main(String[] args){
    ArrayList<Integer> beep = new ArrayList<Integer>();
    beep.add(10);
    beep.add(20);
    beep.add(30);
    mystery2(beep);
  }
}
