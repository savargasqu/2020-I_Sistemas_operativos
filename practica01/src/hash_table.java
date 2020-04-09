import java.io.*;
import java.util.*;
import java.text.*;
import java.math.*;
import java.util.regex.*;

public class RansomNote {

    static class Node<T> {
        T data;
        Node next;

        Node(T data) {
            this.data = data;
        }

        @Override
        public String toString() {
            return String.format("%s ", this.data);
        }

    }

    static class LinkedList<T> {
        Node head;

        LinkedList() {
            this.head = null;
        }

        boolean isEmpty() {
            return head == null;
        }

        void appendHead(Node node) {
            if (this.isEmpty()) {
                this.head = node;
            } else {
                node.next = this.head;
                this.head = node;
            }
        }

        Node removeHead() {
            Node temp = head;
            head = head.next;
            return temp;
        }

        void appendEnd(Node node) {
            if (this.isEmpty()) {
                this.head = node;
            } else {
                Node temp = head;
                while (temp.next != null) {
                    temp = temp.next;
                }
                temp.next = node;
            }
        }

        int size() {
            if (!isEmpty()) {
                Node temp = head;
                int l = 0;
                while (temp.next != null) {
                    temp = temp.next;
                    l += 1;
                }
                return l;
            }
            return 0;
        }

        @Override
        public String toString() {
            if (isEmpty()) {
                return "empty";
            } else {
                Node temp = head;

                StringBuilder s = new StringBuilder();
                s.append("l: " + temp.toString());

                while (temp.next != null) {
                    s.append(temp.toString());
                    temp = temp.next;
                }

                return s.toString();
            }
        }
    }

    static class HashTable {

        ArrayList<LinkedList<String>> buckets;
        /*
         NOTA:
         Para implementar una tabla sin colisiones usando mi propia lista
         se tiene que utilizar un ArrayList en vez de un array simple.
         para evitar un error 'Generic array creation'.
        */
        int size;

        public HashTable(int size) {
            this.size = size;
            this.buckets = new ArrayList<>();
            //this.buckets = new LinkedList<String>[size];
            for (int i = 0; i < size; i++) {
                buckets.add(i, new LinkedList<String>());
            }
        }

        int hashString(String s) {
            // Hash code return polynomial hashing of the String
            // \sum_{i = 0}^{|S| - 1} S[i] \times 31^{i} \mod p
            return Math.abs(s.hashCode()) % size;
        }

        void insert(String s) {
            int h = hashString(s);
            LinkedList l = buckets.get(h);

            //System.out.printf("Insert: s = '%s' | h(s) = %d\n", s, h);

            if (!l.isEmpty()) {
                Node temp = l.head;

                if (temp.data.equals(s)) { // check head
                    return;
                }

                while (temp.next != null) { // check other nodes
                    if (temp.data == s) {
                        return;
                    }
                    temp = temp.next;
                }
            }
            buckets.get(h).appendEnd(new Node<>(s));
        }

        void erase(String s) {
            if (find(s)) {
                int h = hashString(s);
                Node temp = buckets.get(h).head;
                Node prev = null;
                while (temp.next != null) {
                    if (temp.data == s) {
                        prev.next = temp.next;
                    }
                    prev = temp;
                    temp = temp.next;
                }
            }
        }

        boolean find(String s) {
            int h = hashString(s);
            LinkedList l = buckets.get(h);

            //System.out.printf("Find: s = '%s' | h(s) = %d\n", s, h);

            if (!l.isEmpty()) {
                Node temp = l.head;
                if (temp.data.equals(s)) { // check head
                    return true;
                }

                while (temp.next != null) { // check other nodes
                    if (temp.data.equals(s)) {
                        return true;
                    }
                    temp = temp.next;
                }
            }

            return false;
        }

        @Override
        public String toString() {
            String s = String.format("|H| = %d\n", size);
            for (int i = 0; i < size; i++) {
                s += buckets.get(i).toString() + "\n";
            }
            return s;
        }
    }


    // Complete the checkMagazine function below.
    static void checkMagazine(String[] magazine, String[] note) {

        boolean flag = true;
        HashTable ht = new HashTable(magazine.length);

        for (int i = 0; i < magazine.length; i++) {
            ht.insert(magazine[i]);
        }

        for (int j = 0; j < note.length; j++) {
            if (!ht.find(note[j])) {
                flag = false;
                break;
            }
        }

        String result = flag ? "Yes" : "No";
        System.out.println(result);
    }

    private static final Scanner scanner = new Scanner(System.in);

    public static void main(String[] args) {
        String[] mn = scanner.nextLine().split(" ");

        int m = Integer.parseInt(mn[0]);

        int n = Integer.parseInt(mn[1]);

        String[] magazine = new String[m];

        String[] magazineItems = scanner.nextLine().split(" ");
        scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])?");

        for (int i = 0; i < m; i++) {
            String magazineItem = magazineItems[i];
            magazine[i] = magazineItem;
        }

        String[] note = new String[n];

        String[] noteItems = scanner.nextLine().split(" ");
        scanner.skip("(\r\n|[\n\r\u2028\u2029\u0085])?");

        for (int i = 0; i < n; i++) {
            String noteItem = noteItems[i];
            note[i] = noteItem;
        }

        checkMagazine(magazine, note);

        scanner.close();
    }
}
