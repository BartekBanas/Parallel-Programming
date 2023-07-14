public class Wariant2 implements Runnable {
    private Obraz obraz;
    private int id_start, id_end, id;

    public Wariant2(Obraz obraz, int id_start, int id_end, int id) {
        this.id_start = id_start;
        this.id_end = id_end;
        this.obraz = obraz;
        this.id = id;
    }

    @Override
    public void run() {
        for (int i = 0; i < id_end - id_start; i++) {
            obraz.calculate_histogram_watek_v2(id_start + i);
            obraz.print_histogram_watek_v2(id_start + i, id);
        }
    }
}
