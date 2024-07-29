import axios from 'axios';

const DATABASE_URL = "https://iot-parking-system-36475-default-rtdb.firebaseio.com/";

export const getRealtimeData = async (path) => {
    try
    {
        const response = await axios.get(`${DATABASE_URL}${path}.json`);
        return response.data;
    }
    catch (error)
    {
        console.error(`Erro ao obter dados de ${path}:`, error);
        return null;
    }
};
