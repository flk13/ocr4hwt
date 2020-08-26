

using System.Collections.Generic;

namespace ocr1
{
    public class jsonparser
    {
        public class Itemcoord
        {
            public string x { get; set; }
            public string y { get; set; }
            public string width { get; set; }
            public string height { get; set; }
        }

        public class Coords
        {
        }

        public class Words
        {
            public string character { get; set; }
            public string confidence { get; set; }
        }

        public class Candword
        {
        }

        public class Items
        {
            public Itemcoord itemcoord { get; set; }
            public string itemstring { get; set; }
            public List<Coords> coords { get; set; }
            public List<Words> words { get; set; }
            public List<Candword> candword { get; set; }
        }

        public class Data
        {
            public string session_id { get; set; }
            public List<Items> items { get; set; }
        }

        public class RootObject
        {
            public string code { get; set; }
            public string message { get; set; }
            public Data data { get; set; }
        }
    }

    }