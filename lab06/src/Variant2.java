public class Variant2 implements Runnable {
    private final Image image;
    private final int idStart;
    private final int idEnd;
    private final int id;

    public Variant2(Image image, int idStart, int idEnd, int id) {
        this.idStart = idStart;
        this.idEnd = idEnd;
        this.image = image;
        this.id = id;
    }

    @Override
    public void run() {
        for (int i = 0; i < idEnd - idStart; i++) {
            image.calculateHistogramThreadV2(idStart + i);
            image.printHistogramThreadV2(idStart + i, id);
        }
    }
}