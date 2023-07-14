public class Variant1 extends Thread {
    private final Image image;
    private final int id;

    public Variant1(Image image, int id) {
        this.id = id;
        this.image = image;
    }

    @Override
    public void run() {
        image.calculateHistogramThread(this.id);
        image.printHistogramThread(this.id);
    }
}