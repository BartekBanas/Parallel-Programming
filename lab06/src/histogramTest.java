import java.util.Scanner;

class histogramTest
{
    public static void main(String[] args)
    {
        Scanner scanner = new Scanner(System.in);

        System.out.println("Set image size: n (#rows), m(#columns)");
        int rows = scanner.nextInt();
        int columns = scanner.nextInt();
        Image image1 = new Image(rows, columns);

        image1.calculate_histogram();
        image1.print_histogram();

        System.out.println("Set number of threads");
        int threads = scanner.nextInt();

        //Variant1
        System.out.println("Variant 1");
        int threadAmountV1 = 94;
        Wariant1[] threadsVersion1 = new Wariant1[threadAmountV1];

        for (int i = 0; i < threadAmountV1; i++) {
            (threadsVersion1[i] = new Wariant1(image1, i)).start();
        }

        for (int i = 0; i < threadAmountV1; i++) {
            try {
                threadsVersion1[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        //Variant2
        System.out.println("Variant 2");
        Thread[] threadsVersion2 = new Thread[threads];
        double scalar = Math.ceil(94. / threads);
        for (int i = 0; i < threads; i++) {
            int from = (int) scalar * i;
            int to = (int) scalar * (i + 1);
            if (to > 94) to = 94;
            System.out.println(from + " " + to);
            (threadsVersion2[i] = new Thread(new Wariant2(image1, from, to, i))).start();
        }

        for (int i = 0; i < threads; i++) {
            try {
                threadsVersion2[i].join();
            } catch (InterruptedException e) {
                throw new RuntimeException(e);
            }
        }

        image1.compareHistograms();
    }
}