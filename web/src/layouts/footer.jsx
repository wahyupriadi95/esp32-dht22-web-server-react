const VersionNumber = () => {
    const title = import.meta.env.VITE_WEBAPP_ENV === 'production'
        ? `Version ${import.meta.env.VITE_WEBAPP_VERSION} (${import.meta.env.VITE_WEBAPP_ENV})`
        : `Build ${import.meta.env.VITE_WEBAPP_BUILD} (${import.meta.env.VITE_WEBAPP_ENV})`;
    return <small>{title}</small>;
};

const Footer = () => (
    <div className="d-flex mt-auto">
        <span className="ms-auto">
            <small>
                <VersionNumber /> |{' '}
                Wahyu Priadi &copy; 2024.
            </small>
        </span>
    </div>
);

export default Footer;
