import { useEffect, useState } from 'react'
import { parseISO, format } from 'date-fns'
import { id } from 'date-fns/locale'
import './App.css'

function App() {
  const [dht, setDht] = useState(null)
  console.log("🚀 ~ App ~ dht:", dht)
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
      console.log("🚀 ~ handleCount ~ error:", error)
      setLoading(false);
    }
  }

  useEffect(() => {
    getCount()
  }, [])

  return (
    <>
      <header className='bg-blue-400 text-white p-4 md:p-6 sticky top-0'>
        <h1 className='text-2xl text-center md:text-4xl font-bold'>Sensor DHT22</h1>
      </header>
      <main className='container mx-auto mt-6 p-4 mb-4'>
        <section className='border-4 border-blue-600 rounded-lg shadow-md p-6 mb-6'>
          <h2 className='text-sm md:text-2xl font-semibold mb-4'>
            Pembaruan Terakhir : {dht?.serverTime && !loading && format(parseISO(dht?.serverTime), "EEEE, dd MMMM yyyy 'pukul' HH.mm", { locale: id })}
          </h2>
          <button className='bg-blue-600 px-3 py-2 rounded-lg' onClick={getCount}>Perbarui</button>
        </section>
        <section className='flex flex-col md:flex-row w-100 gap-4 mt-2'>
          <div className='flex-auto rounded-lg bg-white p-6 '>
            <h3 className='text-lg md:text-xl text-black'>Suhu</h3>
            <p className='text-xl md:text-2xl text-black'>{dht?.temperature ?? ""}</p>
          </div>
          <div className='flex-auto rounded-lg bg-white p-6 '>
            <h3 className='text-lg md:text-xl text-black'>Kelembapan</h3>
            <p className='text-xl md:text-2xl text-black'>{dht?.humidity ?? "-"}</p>
          </div>
          <a className='text-center mt-2' href=''>Lihat Riwayat</a>
        </section>
      </main>
      <footer className='mt-auto'>
        <p className='text-center'>Wahyu Priadi @ 2024</p>
      </footer>
    </>
  )
}

export default App
