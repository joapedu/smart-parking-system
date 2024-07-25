<template>
  <div>
    <h1>Login</h1>
    <form @submit.prevent="login">
      <input v-model="email" type="email" placeholder="Email" required />
      <input v-model="password" type="password" placeholder="Password" required />
      <button type="submit">Login</button>
    </form>
  </div>
</template>

<script>
import api from '../services/api'

export default
{
  data()
  {
    return {
      email: '',
      password: ''
    }
  },
  methods: {
    async login()
    {
      try
      {
        const response = await api.post('/login', {email: this.email, password: this.password})

        localStorage.setItem('token', response.data.token)
        this.$router.push('/')
      }
      catch (error)
      {
        console.error('Erro no login:', error)
      }
    }
  }
}
</script>
