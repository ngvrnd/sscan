#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QVBoxLayout>
#include <QGroupBox>

/** 
 * This program displays an image and its inverted and grayscale versions; 
 * it also computes and displays the average rgb and greyscale pixel values. 
 * The image file can be provided as a command-line argument or selected via a file dialog.
 *   -ncaruso august 2024
 */

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    //////////////////////////////////////////
    // Check for command-line argument
    QString imagePath;
    QStringList args = app.arguments(); // Get the list of arguments from QApplication
    if (args.size() > 1) {
        imagePath = args[1]; 
    } else {
        // Prompt the user for the image file if no argument is provided
        imagePath = QFileDialog::getOpenFileName(nullptr, "Open Image", QDir::homePath(), "Image Files (*.png *.jpg *.jpeg *.bmp)");
    }

    // Check if the image path is empty, and display a warning message if it is. then exit.
    if (imagePath.isEmpty()) {
        QMessageBox::warning(nullptr, "Warning", "No image file selected.");
        return 1; // Exit if no file is selected
    }

    //////////////////////////////////////////
    // Load the image
    QImage originalImage;
    if (!originalImage.load(imagePath)) {
        QMessageBox::critical(nullptr, "Error", "Could not load image");
        return 1; // Exit with an error code
    }

    //////////////////////////////////////////
    // Compute average pixel values
    int totalValue = 0;
    for (int y = 0; y < originalImage.height(); ++y) {
        for (int x = 0; x < originalImage.width(); ++x) {
            QRgb color = originalImage.pixel(x, y);
            totalValue += qGray(color);
        }
    }
    int averageValue = totalValue / (originalImage.width() * originalImage.height());

    // Compute average pixel values for R, G, and B channels
    int totalRed = 0, totalGreen = 0, totalBlue = 0;
    for (int y = 0; y < originalImage.height(); ++y) {
        for (int x = 0; x < originalImage.width(); ++x) {
            QRgb color = originalImage.pixel(x, y);
            totalRed += qRed(color);
            totalGreen += qGreen(color);
            totalBlue += qBlue(color);
        }
    }
    int averageRed = totalRed / (originalImage.width() * originalImage.height());
    int averageGreen = totalGreen / (originalImage.width() * originalImage.height());
    int averageBlue = totalBlue / (originalImage.width() * originalImage.height());


    //////////////////////////////////////////
    // Main Window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Image Processor");

    // Central Widget with Original Image and Averages
    QGroupBox* centralWidget = new QGroupBox("Original Image", &mainWindow);
    mainWindow.setCentralWidget(centralWidget);

    QLabel imageLabel;
    imageLabel.setPixmap(QPixmap::fromImage(originalImage));

    QLabel averageLabel;
    averageLabel.setText(QString("Average Greyscale Pixel Value: %1").arg(averageValue));

    QLabel averageRGBLabel;
    averageRGBLabel.setText(QString("Average RGB Pixel Values: R=%1, G=%2, B=%3").arg(averageRed).arg(averageGreen).arg(averageBlue));

    QVBoxLayout* centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->addWidget(&imageLabel);
    centralLayout->addWidget(&averageLabel);
    centralLayout->addWidget(&averageRGBLabel);

    // Inverted Image Widget
    QGroupBox* invertedWidget = new QGroupBox("Inverted Image", &mainWindow);

    QImage invertedImage = originalImage.copy();
    invertedImage.mirror(true, true);
    QLabel* invertedLabel = new QLabel(invertedWidget);
    invertedLabel->setPixmap(QPixmap::fromImage(invertedImage));

    QVBoxLayout* invertedLayout = new QVBoxLayout(invertedWidget);
    invertedLayout->addWidget(invertedLabel);

    // Grayscale Image Widget
    QGroupBox* grayscaleWidget = new QGroupBox("Grayscale Image", &mainWindow);

    QImage grayscaleImage = originalImage.convertToFormat(QImage::Format_Grayscale8);
    QLabel* grayscaleLabel = new QLabel(grayscaleWidget);
    grayscaleLabel->setPixmap(QPixmap::fromImage(grayscaleImage));

    QVBoxLayout* grayscaleLayout = new QVBoxLayout(grayscaleWidget);
    grayscaleLayout->addWidget(grayscaleLabel);

    // Add widgets to the main window using a horizontal layout
    QWidget* containerWidget = new QWidget(&mainWindow);
    QHBoxLayout* mainLayout = new QHBoxLayout(containerWidget);
    mainLayout->addWidget(centralWidget);
    mainLayout->addWidget(invertedWidget);
    mainLayout->addWidget(grayscaleWidget);
    mainWindow.setCentralWidget(containerWidget);


    mainWindow.show();

    return app.exec();
}