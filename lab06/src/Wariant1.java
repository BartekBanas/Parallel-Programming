public class Wariant1 extends Thread {
    private Image image;
    private int id;

    public Wariant1(Image image, int id) {
        this.id = id;
        this.image = image;
    }

    @Override
    public void run() {
        image.calculateHistogramThread(this.id);
        image.printHistogramThread(this.id);
    }
    /*
    public static void main(String args[]) {
        (new Wariant1A()).start();
    }

     */
}
