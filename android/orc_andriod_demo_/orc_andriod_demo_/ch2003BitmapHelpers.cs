using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.Graphics.Drawables;
using Android.Graphics;


namespace orc_andriod_demo_
{
    public static class ch2003BitmapHelpers
    {
        public static void RecycleBitmap(ImageView imageView)
        {
            if (imageView == null)
            {
                return;
            }

            Drawable toRecycle = imageView.Drawable;
            if (toRecycle != null)
            {
                ((BitmapDrawable)toRecycle).Bitmap.Recycle();
            }
        }

        public static Bitmap LoadAndResizeBitmap(string fileName, int width, int height)
        {
            BitmapFactory.Options options = new BitmapFactory.Options
            {
                InPurgeable = true,
                InJustDecodeBounds = true
            };
            BitmapFactory.DecodeFile(fileName, options);
            int outHeight = options.OutHeight;
            int outWidth = options.OutWidth;
            int inSampleSize = 1;
            if (outHeight > height || outWidth > width)
            {
                if (outWidth > outHeight)
                {
                    inSampleSize = outHeight / height;
                }
                else
                {
                    inSampleSize = outWidth / width;
                }
            }
            options.InSampleSize = inSampleSize;
            options.InJustDecodeBounds = false;
            Bitmap resizedBitmap = BitmapFactory.DecodeFile(fileName, options);
            return resizedBitmap;
        }
    }
}