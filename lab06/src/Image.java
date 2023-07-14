import java.util.Random;

class Image
{
    private int rows;
    private int columns;
    private char[][] array;
    private char[] charArray;
    private int[] histogram;
    private int[] histogramParallel;
    private int[] histogramParallelV2;

    public Image(int rows, int columns)
    {
        this.rows = rows;
        this.columns = columns;
        array = new char[rows][columns];
        charArray = new char[94];

        final Random random = new Random();

        // for general case where symbols could be not just integers
        for (int k = 0; k < 94; k++) {
            charArray[k] = (char) (k + 33); // substitute symbols
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                array[i][j] = charArray[random.nextInt(94)];  // ascii 33-127
                //tab[i][j] = (char)(random.nextInt(94)+33);  // ascii 33-127
                System.out.print(array[i][j] + " ");
            }

            System.out.print("\n");
        }

        System.out.print("\n\n");

        histogram = new int[94];
        histogramParallel = new int[94];
        histogramParallelV2 = new int[94];
        clear_histogram();
    }

    public void clear_histogram()
    {
        for (int i = 0; i < 94; i++) histogram[i] = 0;
        for (int i = 0; i < 94; i++) histogramParallel[i] = 0;
        for (int i = 0; i < 94; i++) histogramParallelV2[i] = 0;
    }

    public void calculate_histogram()
    {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                for (int k = 0; k < 94; k++) {
                    if (array[i][j] == charArray[k]) histogram[k]++;
                    //if(tab[i][j] == (char)(k+33)) histogram[k]++;
                }
            }
        }
    }

    public void compareHistograms() {
        boolean correct1 = true, correct2 = true;
        for (int i = 0; i < 94; i++) {
            if (histogram[i] != histogramParallel[i])
                correct1 = false;
            if (histogram[i] != histogramParallelV2[i])
                correct2 = false;
        }
        System.out.println(correct1 ? "Histogram Variant 1 - OK" : "Histogram Variant 1 - error");
        System.out.println(correct2 ? "Histogram Variant 2 - OK" : "Histogram Variant 2 - error");

    }

    public void calculateHistogramThread(int id) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (array[i][j] == (char) (id + 33)) histogramParallel[id]++;
            }
        }
    }

    public synchronized void printHistogramThread(int id) {
        System.out.print("Variant 1: Thread " + id + ": " + (char) (id + 33) + " " + histogramParallel[id] + " ");
        for (int i = 0; i < histogramParallel[id]; i++) {
            System.out.print("=");
        }
        System.out.println();
    }

    public void calculateHistogramThreadV2(int id) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                if (array[i][j] == (char) (id + 33)) histogramParallelV2[id]++;
            }
        }
    }

    public synchronized void printHistogramThreadV2(int character, int threadId) {
        System.out.print("Variant 2: Thread " + (threadId + 1) + ": " + (char) (character + 33) + " " + histogramParallelV2[character] + " ");
        for (int i = 0; i < histogramParallelV2[character]; i++) {
            System.out.print("=");
        }
        System.out.println();
    }

    public void print_histogram() {

        for (int i = 0; i < 94; i++) {
            System.out.print(charArray[i] + " " + histogram[i] + "\n");
            //System.out.print((char)(i+33)+" "+histogram[i]+"\n");
        }
    }
}