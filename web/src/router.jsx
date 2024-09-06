/* eslint-disable react-refresh/only-export-components */
import React from 'react';

const Home = React.lazy(() => import('pages/home'));
const TemperatureHistory = React.lazy(() => import('pages/temperature'));
const HumidityHistory = React.lazy(() => import('pages/humidity'));

const routes = [
    { exact: true, path: '/', name: 'Home', component: Home },
    { exact: true, path: '/temperature', name: 'Riwayat Temperatur', component: TemperatureHistory, access: 'beranda', action: 'read' },
    { exact: true, path: '/humidity', name: 'Riwayat Kelembapan', component: HumidityHistory, access: 'beranda', action: 'read' },
];

export default routes;
