<template>
  <header>
    <h1>Minha Aplicação</h1>
    <nav>
      <router-link to="/">Home</router-link>
      <router-link to="/login">Login</router-link>
      <router-link to="/create-user">Create User</router-link>
    </nav>
  </header>
</template>

<script>
export default
{
  name: 'AppHeader'
}
</script>

<style scoped>
header
{
  background: #42b983;
  padding: 16px;
  color: white;
}

nav
{
  margin-top: 8px;
}

nav a
{
  margin: 0 16px;
  text-decoration: none;
  color: white;
}

nav a.router-link-active
{
  font-weight: bold;
}
</style>
