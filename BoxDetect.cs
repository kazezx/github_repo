using AForge;
using AForge.Imaging;
using AForge.Math.Geometry;
using System;
using System.Collections.Generic;
using System.Drawing;

namespace BoxDetector
{
    class Program
    {
        static void Main(string[] args)
        {            
            // Open your image
            string path = @"Images\test_image.png";
            Bitmap image = (Bitmap)Bitmap.FromFile(path);
            int shapes_count = getRectangleCount(image);
            Console.WriteLine(shapes_count);
            Console.ReadLine();
        }

        private static int getRectangleCount(Bitmap image)
        {
            int counter = -1;
            // locating objects
            BlobCounter blobCounter = new BlobCounter();
            blobCounter.FilterBlobs = true;
            blobCounter.MinHeight = 5;
            blobCounter.MinWidth = 5;
            blobCounter.ProcessImage(image);
            Blob[] blobs = blobCounter.GetObjectsInformation();
            // check for rectangles
            SimpleShapeChecker shapeChecker = new SimpleShapeChecker();
            foreach (var blob in blobs)
            {
                List<IntPoint> edgePoints = blobCounter.GetBlobsEdgePoints(blob);
                List<IntPoint> cornerPoints;
                // use the shape checker to extract the corner points
                if (shapeChecker.IsQuadrilateral(edgePoints, out cornerPoints))
                {
                    // only do things if the corners form a rectangle
                    if (shapeChecker.CheckPolygonSubType(cornerPoints) == PolygonSubType.Rectangle)
                    {
                        List<System.Drawing.Point> Points = new List<System.Drawing.Point>();
                        foreach (var point in cornerPoints)
                        {
                            Points.Add(new System.Drawing.Point(point.X, point.Y));
                        }
                        Graphics g = Graphics.FromImage(image);
                        g.DrawPolygon(new Pen(Color.Red, 5.0f), Points.ToArray());
                        counter++;
                        Console.ReadLine();
                    }
                }
            }
            return counter;
        }
    }
}
