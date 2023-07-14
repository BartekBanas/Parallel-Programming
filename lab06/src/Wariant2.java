public class Wariant2 implements Runnable {
    private Image image;
    private int id_start, id_end, id;

    public Wariant2(Image image, int id_start, int id_end, int id) {
        this.id_start = id_start;
        this.id_end = id_end;
        this.image = image;
        this.id = id;
    }

    @Override
    public void run() {
        for (int i = 0; i < id_end - id_start; i++) {
            image.calculateHistogramThreadV2(id_start + i);
            image.printHistogramThreadV2(id_start + i, id);
        }
    }
}
