import { format, parseISO } from 'date-fns';
import { id } from 'date-fns/locale';
import { useEffect, useState } from 'react';
import { Link } from 'react-router-dom';

const roundNumber = (num, dec) => (Math.round((num) * 100) / 100).toFixed(dec);

const App = () => {
    const [dht, setDht] = useState(null);
    const [loading, setLoading] = useState(false);

    const getCount = async () => {
        setLoading(true);
        try {
            const res = await fetch('/api/get-count');
            const data = await res.json();
            if (res.status === 200) {
                setDht(() => data);
            }
            setLoading(false);
        } catch (error) {
            console.log('🚀 ~ handleCount ~ error:', error);
            setLoading(false);
        }
    };

    useEffect(() => {
        getCount();
    }, []);

    return (
        <main className="container mx-auto mt-6 p-4 mb-4">
            <section className="border-4 border-blue-600 rounded-lg shadow-md p-6 mb-6">
                <h2 className="text-sm md:text-2xl font-semibold mb-4">
                    Pembaruan Terakhir : {dht?.serverTime && !loading && format(parseISO(dht?.serverTime), "EEEE, dd MMMM yyyy 'pukul' HH.mm", { locale: id })}
                </h2>
                <button type="button" className="bg-blue-600 px-3 py-2 rounded-lg" onClick={getCount}>Perbarui</button>
            </section>
            <section className="flex flex-col md:flex-row w-100 gap-4 mt-2">
                <div className="flex-auto rounded-lg bg-white p-6 ">
                    <h3 className="text-lg md:text-xl text-black">Suhu</h3>
                    <p className="text-xl md:text-2xl text-black">{dht?.temperature ? `${roundNumber(dht?.temperature, 2)}°C` : '-'}</p>
                    <Link to="/temparature" className="text-blue-500">Riwayat</Link>
                </div>
                <div className="flex-auto rounded-lg bg-white p-6 ">
                    <h3 className="text-lg md:text-xl text-black">Kelembapan</h3>
                    <p className="text-xl md:text-2xl text-black">{dht?.humidity ? `${roundNumber(dht?.humidity, 2)}%` : '-'}</p>
                    <Link to="/humidity" className="text-blue-500">Riwayat</Link>
                </div>
            </section>
            <section>
                {/* <a className="text-center mt-2" href="">Lihat Riwayat</a> */}
            </section>
        </main>
    );
};

export default App;
