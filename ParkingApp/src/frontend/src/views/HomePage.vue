<template>
  <div>
    <h1>Home Page</h1>
    <button @click="logout">Logout</button>
  </div>
</template>

<script>
import api from '../services/api'

export default
{
  methods: {
    async logout()
    {
      try
      {
        const token = localStorage.getItem('token')

        await api.post('/logout', {}, {
          headers: {
            Authorization: `Bearer ${token}`
          }
        })

        localStorage.removeItem('token')
        this.$router.push('/login')
      }
      catch (error)
      {
        console.error('Erro ao fazer logout:', error)
      }
    }
  }
}
</script>
