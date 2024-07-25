import { createApp } from 'vue'
import App from './App.vue'
import router from '@/router/index'
import api from '@/services/api'

const app = createApp(App)

api.interceptors.request.use(config => {
    const token = localStorage.getItem('token')

    if (token)
    {
        config.headers.Authorization = `Bearer ${token}`
    }
    return config
})

app.use(router)
app.mount('#app')
