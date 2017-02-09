/*
 * long type, 13 bit (10 bit ADC + 3 bit cyclesAveragedIR/gain)
 * http://www.schwietering.com/jayduino/filtuino/index.php?characteristic=be&passmode=lp&order=3&usesr=usesr&sr=2000&frequencyLow=30&noteLow=&noteHigh=&pw=pw&calctype=long&bitres=13&run=Send
 */

//Low pass chebyshev filter order=3 alpha1=0.015 
class filter
{
    private:
        int16_t v[4];
        int16_t _gain;
    public:
        filter(uint16_t gain)
        {
            _gain = gain;
        }
        void init(void)
        {
            for(int i=0; i <= 3; i++)
                v[i]=0;
        }
        void addReading(uint16_t x)
        {
            v[0] = v[1];
            v[1] = v[2];
            v[2] = v[3];
            int32_t tmp = (((((int32_t)x * _gain * 219131L) >> 17)  //= (   2.5510230385e-5 * x)
                + ((v[0] * 247798L) >> 2)   //+(  0.9452729722*v[0])
                + (v[1] * -188916L) //+( -2.8826321053*v[1])
                + (v[2] * 192489L)  //+(  2.9371550513*v[2])
                )+32768) >> 16; // round and downshift fixed point /65536

            v[3]= (int16_t)tmp;
        }
        uint16_t get(void)
        {
            int32_t tmp = (((/* xpart */
                 (((v[0] + v[3]))<<15) /* 131072L (^2)*/
                 + /* xi==xj*/(196608L * (v[1] + v[2]))
                )
+32768) >> 16); // round and downshift fixed point
            return (tmp > 0) ? (uint16_t)tmp : 0;
        }
};
