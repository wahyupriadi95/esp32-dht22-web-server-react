import 'App.css';
import { createBrowserHistory } from 'history';
import DefaultLayout from 'layouts';
import { Suspense } from 'react';
import { Route, BrowserRouter as Router, Routes } from 'react-router-dom';
import AppRoutes from 'router';

const history = createBrowserHistory();

const App = () => {
    const loading = () => {};

    return (
        <Router basename="/" history={history}>
            <Suspense fallback={loading()}>
                <Routes>
                    {AppRoutes.map((route, idx) => {
                        const Component = route?.component;
                        return Component ? (
                            <Route
                                key={idx}
                                {...route}
                                path={route.path}
                                exact={route.exact}
                                name={route.name}
                                element={(
                                    <DefaultLayout>
                                        <Component title={route.name} />
                                    </DefaultLayout>
                                )}
                            />
                        ) : null;
                    })}
                </Routes>
            </Suspense>
        </Router>
    );
};

export default App;
