import { initializeApp } from "firebase/app";
import { getDatabase } from "firebase/database";

const firebaseConfig = {
    databaseURL: ""
};

const app = initializeApp(firebaseConfig);
const database = getDatabase(app);

export { database };
