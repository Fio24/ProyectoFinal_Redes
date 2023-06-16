import { Injectable } from '@angular/core';
import { HttpClient, HttpHeaders } from '@angular/common/http';
import { Observable, take } from 'rxjs';
import { environment } from 'src/environments/environment';

@Injectable({
  providedIn: 'root'
})
export class DnsService {

  constructor(private httpClient: HttpClient) { }

  getAll(){
    return this.httpClient
    //.get("/api/getAll")
    .get("http://localhost:8080/getAll")
    .pipe(take(1))
  }

  new( register:any){
    return this.httpClient
    .put("http://localhost:8080/new", register)
    .pipe(take(1))
  }

  delete(url:string){
    const options = {
      headers: new HttpHeaders({
        'Content-Type': 'application/json',
      }),
      body: {
        "url":url
      },
    };
    return this.httpClient
    .delete("http://localhost:8080/del",options )
    .pipe(take(1))
  }

  getById(id:string){
    return this.httpClient
    .get(`http://localhost:8080/get/${id}`)
    .pipe(take(1))
  }

  update( register:any){
    return this.httpClient
    .post("http://localhost:8080/update", register)
    .pipe(take(1))
  }
}


