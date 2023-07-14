import java.util.Scanner;

class Histogram_test {

    public static void main(String[] args) {

        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#kolumns)");
        int n = scanner.nextInt();
        int m = scanner.nextInt();
        Obraz obraz_1 = new Obraz(n, m);

        obraz_1.calculate_histogram();
        obraz_1.print_histogram();

        System.out.println("Set number of threads");
        int num_threads = scanner.nextInt();

        //wariant1
        System.out.println("Wariant 1");
        int num_threads_w1 = 94;
        Wariant1[] NewThr = new Wariant1[num_threads_w1];

        for (int i = 0; i < num_threads_w1; i++) {
            (NewThr[i] = new Wariant1(obraz_1, i)).start();
        }
        for (int i = 0; i < num_threads_w1; i++) {
            try {
                NewThr[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        //wariant2
        System.out.println("Wariant 2");
        Thread[] NewThr_v2 = new Thread[num_threads];
        double scalar = Math.ceil(94. / num_threads);
        for (int i = 0; i < num_threads; i++) {
            int from = (int) scalar * i;
            int to = (int) scalar * (i + 1);
            if (to > 94) to = 94;
            System.out.println(from + " " + to);
            (NewThr_v2[i] = new Thread(new Wariant2(obraz_1, from, to, i))).start();
        }

        for (int i = 0; i < num_threads; i++) {
            try {
                NewThr_v2[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        obraz_1.compare_histograms();
    }
}

