Rails.application.routes.draw do
  resources :devices
  get '/usages/:id', to: 'usages#usage_by_device'
  post '/usages', to: 'usages#create_for_device'
  delete '/usages/:id', to: 'usages#reset_by_device'
  # For details on the DSL available within this file, see http://guides.rubyonrails.org/routing.html
end
