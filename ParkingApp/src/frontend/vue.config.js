const { defineConfig } = require('@vue/cli-service')

module.exports = defineConfig({
    transpileDependencies: true,

    devServer: {
        port: 3000,
        proxy: {
            '/auth': {
                target: 'http://localhost:8080',
                changeOrigin: true,
                secure: false,
                pathRewrite: {
                    '^/auth': '/auth',
                },
            },
        },
    },
})
