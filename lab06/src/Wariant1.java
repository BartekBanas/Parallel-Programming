public class Wariant1 extends Thread {
    private Obraz obraz;
    private int id;

    public Wariant1(Obraz obraz, int id) {
        this.id = id;
        this.obraz = obraz;
    }

    @Override
    public void run() {
        obraz.calculate_histogram_watek(this.id);
        obraz.print_histogram_watek(this.id);
    }
    /*
    public static void main(String args[]) {
        (new Wariant1A()).start();
    }

     */
}
